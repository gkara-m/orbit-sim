#pragma once

#include <vector>
#include <array>

// internal logic structs
struct Physics {
  double g {};
  double dt {};
};
struct Settings {
  int gui {};
  bool use_steps {};
  int steps {};
  int integrator {};
  int algorithm {};
  double bh_theta {};
};
struct State {
  Physics physics {};
  std::vector<double> masses {};
  std::vector<double> positions {};
  std::vector<double> velocities {};
  std::vector<double> accelerations {};
  std::array<double, 4> system_dimensions {}; // 0 = width, 1 = height, [3,4] = [centre_x, centre_y]
};
struct Config {
  State state {};
  Settings settings {};
};

// ui interfacing structs
struct SharedState {
  State state {};
  bool keep_running {true};
};
