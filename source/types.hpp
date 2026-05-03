#pragma once

#include <shared_mutex>
#include <vector>

// internal logic structs
struct Physics {
  double g {};
  double dt {};
};
struct Settings {
  int gui {};
  bool use_steps {};
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

// ui interfacing structs
struct SharedState {
  std::shared_mutex mutex {};
  State state {};
};
