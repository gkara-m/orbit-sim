#include <nlohmann/json.hpp>

#include "math_utils.hpp"

#include <fstream>
#include <iostream>
#include <array>
#include <cmath>
#include <vector>

using json = nlohmann::json;

auto get_distance(const std::array<double, 2>& body1, const std::array<double, 2>& body2) -> double {
  std::array<double, 2> vector { body1 - body2 };
  double distance { std::hypot(vector[0], vector[1]) };

  return distance;
}

auto get_force_scalar(const double mass1, const double mass2, const double distance, const double g) -> double {
  double force = { g * mass1 * mass2 / (distance * distance) };

  return force;
}

auto get_unit_direction(const std::array<double, 2>& body1, const std::array<double, 2>& body2, double distance) -> std::array<double, 2> {
  std::array<double, 2> unit_direction { (body1 - body2) / distance };

  return unit_direction;
}

void vv_update_pos(std::array<double, 2>& pos, const std::array<double, 2>& vel, const std::array<double, 2>& acc, const double dt) {
  pos+= (vel * dt + acc * dt * dt / 2);
}

std::array<double, 2> vv_get_acc_new(const std::vector<Body>& bodies, const Body& body_current, const double g) {

  std::vector<std::array<double, 2>> accelerations {};
  for (const Body& body: bodies) {
    if (body != body_current) {
      double distance { get_distance(body.position, body_current.position) };
      double force_scalar { get_force_scalar(body.mass, body_current.mass, distance, g) };
      std::array<double, 2> unit_direction { get_unit_direction(body.position, body_current.position, distance) };
      std::array<double, 2> force_vector { unit_direction * force_scalar };
      accelerations.push_back( force_vector / body_current.mass);
    };
  };

  std::array<double, 2> acc_total {0,0};
  for (const std::array<double, 2>& acc: accelerations) {
    acc_total += acc;
  };
  return acc_total;
} // TODO refactor to not need accelerations vector

void vv_update_vel(std::array<double, 2>& vel, const std::array<double, 2>& acc_old, const std::array<double, 2>& acc_new, const double dt) {
  vel += (acc_old + acc_new) * dt / 2;
}

void velocity_verlet(std::vector<Body>& bodies, const double dt, const double g) {
  for (Body& body: bodies) {
    vv_update_pos(body.position, body.velocity, body.acceleration, dt);
  };

  for (Body& body: bodies) {
    
    std::array<double, 2> acceleration_new { vv_get_acc_new(bodies, body, g) };
    vv_update_vel(body.velocity, body.acceleration, acceleration_new, dt);
    body.acceleration = acceleration_new;
  };


  std::cout << "\n new positions: [" << bodies[0].position[0] << ", " << bodies[0].position[1] << "], 1[" << bodies[1].position[0] << ", " << bodies[1].position[1] << "].";
}

Config startup() {
  std::ifstream file("/home/user/dev/c++/orbit-sim/data/config.json");
  json j {};
  file >> j;
  Config config {};
  from_json(j, config);
  return config;
}

auto main() -> int {
  Config conf { startup() };
  std::cout << "Loaded config.json" << "\n";
  
  std::cout << "Initial Positions: ";
  for (Body& body: conf.bodies) {
    std::cout << "[" << body.position[0] << "," << body.position[1] << "] ";
  };
  std::cout << "\n";
  
  for (int i = 0; i < conf.settings.steps; i++) {
    velocity_verlet(conf.bodies, conf.settings.dt, conf.settings.g);
    for (Body& body: conf.bodies) {
      std::cout << "[" << body.position[0] << "," << body.position[1] << "] ";
    };
    std::cout << "\n";
  }

  return 0;
}

