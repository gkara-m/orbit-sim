#include <iostream>
#include <napi.h>
#include "sim_entry.hpp"

auto napi_extract_array(const Napi::Array& ts_array) {
  double x {ts_array.Get(int(0)).As<Napi::Number>()};
  double y {ts_array.Get(int(1)).As<Napi::Number>()};
  return std::array<double, 2> {x, y};
}

Napi::Value start_sim(const Napi::CallbackInfo& info) {
  Napi::Env env { info.Env() };

  Napi::Object ts_config { info[0].As<Napi::Object>() };

  Napi::Object ts_settings { ts_config.Get("settings").As<Napi::Object>() };
  double g { ts_settings.Get("g").As<Napi::Number>() };
  double dt { ts_settings.Get("dt").As<Napi::Number>() };
  int steps { ts_settings.Get("steps").As<Napi::Number>().Int32Value() };
  Settings settings {g, dt, steps};

  Napi::Array ts_bodies { ts_config.Get("bodies").As<Napi::Array>() };
  std::vector<Body> bodies {};
  for (int i = 0; i < ts_bodies.Length(); i++) {
    Napi::Object ts_body {ts_bodies.Get(i).As<Napi::Object>()};
    Body body {};
    body.id = ts_body.Get("id").As<Napi::Number>().Int32Value();
    body.mass = ts_body.Get("mass").As<Napi::Number>();
    body.position = napi_extract_array(ts_body.Get("position").As<Napi::Array>());
    body.velocity = napi_extract_array(ts_body.Get("velocity").As<Napi::Array>());
    body.acceleration = napi_extract_array(ts_body.Get("acceleration").As<Napi::Array>());
    bodies.push_back(body);
  }
  
  Config conf {settings, bodies};
  int exit_code {sim_entry(conf)};
  if (exit_code != 0) {
    std::cout << exit_code;
  };

  return Napi::String::New(env, "Simulation bridge initialised");
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "start"), 
                Napi::Function::New(env, start_sim));
    return exports;
}

NODE_API_MODULE(orbit_sim, Init)
