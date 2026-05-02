#pragma once
#include <vector>
#include <array>

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
