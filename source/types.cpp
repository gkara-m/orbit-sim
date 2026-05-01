#include <nlohmann/json.hpp>
#include "types.hpp"

using json = nlohmann::json;

void from_json(const json& j, Body& body) {
  j.at("id").get_to(body.id);
  j.at("mass").get_to(body.mass);
  j.at("position").get_to(body.position);
  j.at("velocity").get_to(body.velocity);
  j.at("acceleration").get_to(body.acceleration);
}
void from_json(const json& j, Settings& settings) {
  j.at("g").get_to(settings.g);
  j.at("dt").get_to(settings.dt);
  j.at("steps").get_to(settings.steps);
}
void from_json(const json& j, Config& conf) {
  j.at("settings").get_to(conf.settings);
  j.at("bodies").get_to(conf.bodies);
}
