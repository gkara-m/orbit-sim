#pragma once
#include <vector>
#include <array>
#include <nlohmann/json_fwd.hpp>
using json = nlohmann::json;

struct Body {
  int id {};
  double mass {};
  std::array<double, 2> position;
  std::array<double, 2> velocity;
  std::array<double, 2> acceleration;
};
struct Settings {
  double g;
  double dt;
  int steps;
};
struct Config {
  Settings settings;
  std::vector<Body> bodies;
};

void from_json(const json& j, Body& body);
void from_json(const json& j, Settings& settings);
void from_json(const json& j, Config& config);
