#include <napi.h>
#include <thread>
#include "sim_entry.hpp"

std::shared_ptr<SharedState> globalData {std::make_shared<SharedState>()};

Config ts_to_conf(const Napi::Object& ts_config) {

  // parse received from typescript
  Napi::Object ts_settings { ts_config.Get("settings").As<Napi::Object>() };
  int gui { ts_settings.Get("gui").As<Napi::Number>().Int32Value() };
  bool use_steps { ts_settings.Get("useSteps").As<Napi::Boolean>() };
  int steps { ts_settings.Get("steps").As<Napi::Number>().Int32Value() };
  int integrator { ts_settings.Get("integrator").As<Napi::Number>().Int32Value() };
  int algorithm { ts_settings.Get("algorithm").As<Napi::Number>().Int32Value() };
  Settings settings { gui, use_steps, steps, integrator, algorithm };

  Napi::Object ts_physics { ts_config.Get("physics").As<Napi::Object>() };
  double g { ts_physics.Get("g").As<Napi::Number>() };
  double dt { ts_physics.Get("dt").As<Napi::Number>() };
  Physics physics {g, dt};
  
  // parse every body and push parsed to vector
  Napi::Array ts_bodies { ts_config.Get("bodies").As<Napi::Array>() };
  std::vector<double> masses {};
  std::vector<double> positions {};
  std::vector<double> velocities {};
  std::vector<double> accelerations {};
  for (int i {0}; i < ts_bodies.Length(); ++i) {
    Napi::Object ts_body {ts_bodies.Get(i).As<Napi::Object>()};
    masses.push_back(ts_body.Get("mass").As<Napi::Number>());
    Napi::Array position_array {ts_body.Get("position").As<Napi::Array>()};
    for (int j {0}; j < position_array.Length(); ++j) {
      positions.push_back(position_array.Get(j).As<Napi::Number>());
    };
    Napi::Array velocity_array {ts_body.Get("velocity").As<Napi::Array>()};
    for (int j {0}; j < velocity_array.Length(); ++j) {
      velocities.push_back(velocity_array.Get(j).As<Napi::Number>());
    };
    Napi::Array acceleration_array {ts_body.Get("acceleration").As<Napi::Array>()};
    for (int j {0}; j < acceleration_array.Length(); ++j) {
      accelerations.push_back(acceleration_array.Get(j).As<Napi::Number>());
    };
  };

  // TODO
  double width {0};
  double height {0};

  State state { physics, masses, positions, velocities, accelerations, width, height };
  
  return Config { state, settings };
}

Napi::Value get_buffer(const Napi::CallbackInfo& info) {
  Napi::Env env { info.Env() };
  Napi::Object buffers {Napi::Object::New(env)};

  auto create_view_vec {[&](std::vector<double>& vec) {
    if (vec.empty()) return Napi::Float64Array();
    size_t byte_length {vec.size() * sizeof(double)};
    Napi::ArrayBuffer buffer {Napi::ArrayBuffer::New(env, vec.data(), byte_length)};
    return Napi::Float64Array::New(env, vec.size(), buffer, 0);
  }};

  auto create_view_arr4 {[&](std::array<double, 4>& arr) {
    if (arr.empty()) return Napi::Float64Array();
    size_t byte_length {arr.size() * sizeof(double)};
    Napi::ArrayBuffer buffer {Napi::ArrayBuffer::New(env, arr.data(), byte_length)};
    return Napi::Float64Array::New(env, arr.size(), buffer, 0);
  }};

  auto create_view_float {[&](double& num) {
    size_t byte_length {sizeof(double)};
    Napi::ArrayBuffer buffer {Napi::ArrayBuffer::New(env, &num, byte_length)};
    return Napi::Float64Array::New(env, 1, buffer, 0);
  }};

  auto create_view_bool {[&](bool& boolean) {
    size_t byte_length {sizeof(bool)};
    Napi::ArrayBuffer buffer {Napi::ArrayBuffer::New(env, &boolean, byte_length)};
    return Napi::Uint8Array::New(env, 1, buffer, 0);
  }};

  buffers.Set("positions", create_view_vec(globalData->state.positions));
  buffers.Set("velocities", create_view_vec(globalData->state.velocities));
  buffers.Set("accelerations", create_view_vec(globalData->state.accelerations));
  buffers.Set("g", create_view_float(globalData->state.physics.g));
  buffers.Set("dt", create_view_float(globalData->state.physics.dt));
  buffers.Set("dimensions", create_view_arr4(globalData->state.system_dimensions));

  buffers.Set("keepRunning", create_view_bool(globalData->keep_running));

  return buffers;
};

Napi::Value start_sim(const Napi::CallbackInfo& info) {
  Napi::Env env { info.Env() };

  // receive startup info from typescript
  Napi::Object ts_config { info[0].As<Napi::Object>() };
  Config temp_conf { ts_to_conf(ts_config) };
  globalData->state = std::move(temp_conf.state);

  // spin off main simulation thread
  auto runtime_thread {
    std::thread([temp_conf]() {
      sim_entry(globalData, temp_conf.settings);
    })
  };
  runtime_thread.detach();

  return Napi::Number::New(env, 0);
}

// functions available from typescript
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "start"), Napi::Function::New(env, start_sim));
  exports.Set(Napi::String::New(env, "getBuffer"), Napi::Function::New(env, get_buffer));
  return exports;
}

NODE_API_MODULE(orbit_sim, Init);
