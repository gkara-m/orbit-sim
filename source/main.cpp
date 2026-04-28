#include <iostream>
#include <string>
#include <array>
#include <cmath>

constexpr double gravitational_constant {6.6743e-11};

struct Body {
  int id {};
  double mass {};
  std::array<double, 2> position;
  std::array<double, 2> velocity;
};

auto get_distance(const std::array<double, 2>& body1, const std::array<double, 2>& body2) -> double {
  double x { body1[0] - body2[0] };
  double y { body1[1] - body2[1] };
  double distance { std::hypot(x, y) };

  return distance;
}

auto get_force_scalar(const double body1, const double body2, const double distance) -> double {
  double force = { gravitational_constant * body1 * body2 / (distance * distance) };
  return force;
}

auto get_unit_direction(const std::array<double, 2>& body1, const std::array<double, 2>& body2) -> std::array<double, 2> {
  
}

auto main() -> int {

  Body default0 {0, 10, {-1, 0}, {0, 5} };
  Body default1 {1, 10, {1, 0}, {0, -5} };

  double distance { get_distance(default0.position, default1.position) };
  double force_scalar { get_force_scalar(default0.mass, default1.mass, distance)};

  return 0;
}
