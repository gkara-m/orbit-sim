#include <napi.h>
#include <sim_entry.hpp>

Napi::Value start_sim(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  sim_entry();

  return Napi::String::New(env, "Simulation bridge initialised");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "start"), 
                Napi::Function::New(env, start_sim));
    return exports;
}

NODE_API_MODULE(orbital_sim, Init)
