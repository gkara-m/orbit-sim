#include <iostream>
#include <string>
#include <array>
#include <cmath>
#include <vector>

constexpr double gravitational_constant {6.6743e-11};
constexpr double time_interval {60};

struct Body {
  int id {};
  double mass {};
  std::array<double, 2> position;
  std::array<double, 2> velocity;
  std::array<double, 2> acceleration;
};

std::array<double, 2> operator+(const std::array<double, 2>& array1, const std::array<double, 2>& array2) {
  double x { array1[0] + array2[0] };
  double y { array1[1] + array2[1] };

  return std::array<double, 2> {x, y};
}
std::array<double, 2> operator+=(std::array<double, 2>& array1, const std::array<double, 2>& array2) {
  array1[0] += array2[0];
  array1[1] += array2[1];

  return array1;
}
std::array<double, 2> operator-(const std::array<double, 2>& array1, const std::array<double, 2>& array2) {
  double x { array1[0] - array2[0] };
  double y { array1[1] - array2[1] };

  return std::array<double, 2> {x, y};
}
std::array<double, 2> operator*(const std::array<double, 2>& array1, const std::array<double, 2>& array2) {
  double x { array1[0] * array2[0] };
  double y { array1[1] * array2[1] };

  return std::array<double, 2> {x, y};
}
std::array<double, 2> operator/(const std::array<double, 2>& array1, const std::array<double, 2>& array2) {
  double x { array1[0] / array2[0] };
  double y { array1[1] / array2[1] };

  return std::array<double, 2> {x, y};
}
std::array<double, 2> operator*(const std::array<double, 2>& array1, const double multiplier) {
  double x { array1[0] * multiplier };
  double y { array1[1] * multiplier };

  return std::array<double, 2> {x,y};
}
std::array<double, 2> operator/(const std::array<double, 2>& array1, const double multiplier) {
  double x { array1[0] / multiplier };
  double y { array1[1] / multiplier };

  return std::array<double, 2> {x,y};
}

auto get_distance(const std::array<double, 2>& body1, const std::array<double, 2>& body2) -> double {
  std::array<double, 2> vector { body1 - body2 };
  double distance { std::hypot(vector[0], vector[1]) };

  return distance;
}

auto get_force_scalar(const double body1, const double body2, const double distance) -> double {
  double force = { gravitational_constant * body1 * body2 / (distance * distance) };

  return force;
}

auto get_unit_direction(const std::array<double, 2>& body1, const std::array<double, 2>& body2, double distance) -> std::array<double, 2> {
  std::array<double, 2> unit_direction { (body1 - body2) / distance };

  return unit_direction;
}

auto main() -> int {
  Body earth {0, 5.972e24, {0, 0}, {0, 0} };
  Body moon {1, 7.348e21, {3.844e8, 0}, {0, 1022} };
  std::vector<Body> bodies {earth, moon};
  std::cout << "initial-positions: 0[" << earth.position[0] << ", " << earth.position[1] << "], 1[" << moon.position[0] << ", " << moon.position[1] << "].";
  
  for (int i = 0; i < 100; i++) {
    double distance { get_distance(earth.position, moon.position) };
    double force_scalar { get_force_scalar(earth.mass, moon.mass, distance)};
    std::array<double, 2> unit_direction { get_unit_direction(earth.position, moon.position, distance) };
    std::array<double, 2> force_vector_earth { unit_direction * force_scalar };
    std::array<double, 2> force_vector_moon { unit_direction * -force_scalar };

    std::array<double, 2> acceleration_earth { force_vector_earth / earth.mass };
    std::array<double, 2> acceleration_moon { force_vector_moon / moon.mass };
    std::array<double, 2> velocity_earth { earth.velocity + (acceleration_earth * time_interval) };
    std::array<double, 2> velocity_moon { moon.velocity + (acceleration_moon * time_interval) };
    earth.velocity = velocity_earth;
    moon.velocity = velocity_moon;

    std::array<double, 2> position_earth { earth.position + (velocity_earth * time_interval) };
    std::array<double, 2> position_moon { moon.position + (velocity_moon * time_interval) };
    earth.position = position_earth;
    moon.position = position_moon;

    std::cout << "\n position at interval1: 0[" << earth.position[0] << ", " << earth.position[1] << "], 1[" << moon.position[0] << ", " << moon.position[1] << "].";
  }


  return 0;
}

void vv_update_pos(std::array<double, 2>& pos, const std::array<double, 2>& vel, const std::array<double, 2>& acc) {
  pos+= (vel * time_interval + acc * time_interval * time_interval / 2);
}

std::array<double, 2> vv_get_acc_new() {}

void vv_update_vel(std::array<double, 2>& vel, const std::array<double, 2>& acc_old, const std::array<double, 2>& acc_new) {
  vel += (acc_old + acc_new) * time_interval / 2;
}

auto velocity_verlet(std::vector<Body>& bodies) -> void {
  for (Body& body: bodies) {
    vv_update_pos(body.position, body.velocity, body.acceleration);
  };

  for (Body& body: bodies) {
    std::array<double, 2> acceleration_new { vv_get_acc_new() };
    vv_update_vel(body.velocity, body.acceleration, acceleration_new);
    body.acceleration = acceleration_new;
  };
}

