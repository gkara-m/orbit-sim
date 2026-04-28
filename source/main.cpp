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

auto get_unit_direction(const std::array<double, 2>& body1, const std::array<double, 2>& body2, double distance) -> std::array<double, 2> {
  double x { (body1[0]-body2[0]) / distance };
  double y { (body1[1]-body2[1]) / distance };
  std::array<double, 2> unit_direction {x, y};

  return unit_direction;
}

auto get_force_vector(const double force_scalar, const std::array<double, 2>& unit_direction) -> std::array<double, 2> {
  double x { unit_direction[0] * force_scalar };
  double y { unit_direction[1] * force_scalar };
  std::array<double, 2> force_vector { x, y };

  return force_vector;
}

auto get_acceleration_vector(const double mass, const std::array<double, 2>& force_vector) -> std::array<double, 2> {
  double x { force_vector[0] / mass };
  double y { force_vector[1] / mass };
  std::array<double, 2> acceleration_vector = { x, y };

  return acceleration_vector;
}

auto main() -> int {

  Body default0 {0, 1e12, {-1, 0}, {0, 5} };
  Body default1 {1, 1e12, {1, 0}, {0, -5} };
  std::cout << "initial-positions: 0[" << default0.position[0] << ", " << default0.position[1] << "], 1[" << default1.position[0] << ", " << default1.position[1] << "].";

  double distance { get_distance(default0.position, default1.position) };
  double force_scalar { get_force_scalar(default0.mass, default1.mass, distance)};
  std::array<double, 2> unit_direction { get_unit_direction(default0.position, default1.position, distance) };
  std::array<double, 2> force_vector_def0 { get_force_vector(force_scalar, unit_direction) };
  std::array<double, 2> force_vector_def1 { -force_vector_def0[0], -force_vector_def0[1] }; // TODO use operator overloading to replace this
  std::array<double, 2> acceleration_def0 { get_acceleration_vector(default0.mass, force_vector_def0) };
  std::array<double, 2> acceleration_def1 { get_acceleration_vector(default1.mass, force_vector_def1) };

  double time_interval { 0.01 };

  std::array<double, 2> position_def0 { default0.position[0] + acceleration_def0[0] * time_interval * time_interval, default0.position[1] + acceleration_def0[1] * time_interval * time_interval };
  std::array<double, 2> position_def1 { default1.position[0] + acceleration_def1[0] * time_interval * time_interval, default1.position[1] + acceleration_def1[1] * time_interval * time_interval };
  default0.position = position_def0;
  default1.position = position_def1;

  std::cout << "\n position at interval1: 0[" << default0.position[0] << ", " << default0.position[1] << "], 1[" << default1.position[0] << ", " << default1.position[1] << "].";


  return 0;
}
