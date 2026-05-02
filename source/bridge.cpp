#include <iostream>
#include <napi.h>
#include <thread>
#include <vector>
#include "sim_entry.hpp"

Config ts_to_conf(const Napi::Object& ts_config) {

  Napi::Object ts_settings { ts_config.Get("settings").As<Napi::Object>() };
  int gui { ts_settings.Get("gui").As<Napi::Number>().Int32Value() };
  int steps { ts_settings.Get("steps").As<Napi::Number>().Int32Value() };
  Settings settings { gui, steps };


  Napi::Object ts_physics { ts_config.Get("physics").As<Napi::Object>() };
  double g { ts_physics.Get("g").As<Napi::Number>() };
  double dt { ts_physics.Get("dt").As<Napi::Number>() };
  Physics physics {g, dt};
  
  // Parse every body and push parsed to vector
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
  
  State state { physics, masses, positions, velocities, accelerations };
  
  return Config { state, settings };
}

Napi::Value start_sim(const Napi::CallbackInfo& info) {
  Napi::Env env { info.Env() };

  Napi::Object ts_config { info[0].As<Napi::Object>() };
  Config conf { ts_to_conf(ts_config) };

  auto runtime_thread {
    std::thread([conf]() {

      int exit_code {sim_entry(conf)};
      if (exit_code != 0) {
        std::cout << exit_code;
      };
    })
  };

  runtime_thread.detach();
  return Napi::String::New(env, "Simulation bridge initialised");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "start"), 
                Napi::Function::New(env, start_sim));
    return exports;
}

NODE_API_MODULE(orbit_sim, Init)
