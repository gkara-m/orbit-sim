#include <napi.h>
#include <sim_entry.hpp>

Napi::Value start_sim(const Napi::CallbackInfo& info) {
  Napi::Env env { info.Env() };

  Napi::Object config { info[0].As<Napi::Object>() };

  Napi::Object settings { config.Get("settings").As<Napi::Object>() };
  double g { settings.Get("g").As<Napi::Number>() };

  sim_entry();

  return Napi::String::New(env, "Simulation bridge initialised");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "start"), 
                Napi::Function::New(env, start_sim));
    return exports;
}

NODE_API_MODULE(orbit_sim, Init)
