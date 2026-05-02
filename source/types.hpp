#pragma once

#include <vector>

// struct Body {
//   int id {};
//   double mass {};
//   std::array<double, 2> position {};
//   std::array<double, 2> velocity {};
//   std::array<double, 2> acceleration {};
// };
struct Physics {
  double g {};
  double dt {};
};
struct Settings {
  int gui {};
  int steps {};
};
struct State {
  Physics physics {};
  std::vector<double> masses;
  std::vector<double> positions;
  std::vector<double> velocities;
  std::vector<double> accelerations;
};
struct Config {
  State state {};
  Settings settings {};
};
