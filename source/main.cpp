#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <cmath>
#include <vector>

using json = nlohmann::json;

constexpr double gravitational_constant {6.6743e-11};
constexpr double time_interval {3600};

struct Body {
  int id {};
  double mass {};
  std::array<double, 2> position;
  std::array<double, 2> velocity;
  std::array<double, 2> acceleration;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Body, id, mass, position, velocity, acceleration)

struct Settings {
  double g;
  double dt;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Settings, g, dt)

struct Config {
  Settings settings;
  std::vector<Body> bodies;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, settings, bodies)

// ARRAY OPERATOR OVERRIDES
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

// BODY OPERATOR OVERRRIDES
bool operator!=(const Body& body1, const Body& body2) {
  return body1.id != body2.id;
}
bool operator==(const Body& body1, const Body& body2) {
  return body1.id == body2.id;
}

auto get_distance(const std::array<double, 2>& body1, const std::array<double, 2>& body2) -> double {
  std::array<double, 2> vector { body1 - body2 };
  double distance { std::hypot(vector[0], vector[1]) };

  return distance;
}

auto get_force_scalar(const double mass1, const double mass2, const double distance) -> double {
  double force = { gravitational_constant * mass1 * mass2 / (distance * distance) };

  return force;
}

auto get_unit_direction(const std::array<double, 2>& body1, const std::array<double, 2>& body2, double distance) -> std::array<double, 2> {
  std::array<double, 2> unit_direction { (body1 - body2) / distance };

  return unit_direction;
}

void vv_update_pos(std::array<double, 2>& pos, const std::array<double, 2>& vel, const std::array<double, 2>& acc) {
  pos+= (vel * time_interval + acc * time_interval * time_interval / 2);
}

std::array<double, 2> vv_get_acc_new(const std::vector<Body>& bodies, const Body& body_current) {

  std::vector<std::array<double, 2>> accelerations {};
  for (const Body& body: bodies) {
    if (body != body_current) {
      double distance { get_distance(body.position, body_current.position) };
      double force_scalar { get_force_scalar(body.mass, body_current.mass, distance) };
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

void vv_update_vel(std::array<double, 2>& vel, const std::array<double, 2>& acc_old, const std::array<double, 2>& acc_new) {
  vel += (acc_old + acc_new) * time_interval / 2;
}

void velocity_verlet(std::vector<Body>& bodies) {
  for (Body& body: bodies) {
    vv_update_pos(body.position, body.velocity, body.acceleration);
  };

  for (Body& body: bodies) {
    
    std::array<double, 2> acceleration_new { vv_get_acc_new(bodies, body) };
    vv_update_vel(body.velocity, body.acceleration, acceleration_new);
    body.acceleration = acceleration_new;
  };


  std::cout << "\n new positions: [" << bodies[0].position[0] << ", " << bodies[0].position[1] << "], 1[" << bodies[1].position[0] << ", " << bodies[1].position[1] << "].";
}

auto main() -> int {
  std::ifstream file("/home/user/dev/c++/orbital_sim/data/config.json");
  json j {};
  file >> j;

  Config conf {j.get<Config>()};
  std::cout << "Loaded g: " << conf.settings.g << "\n";
  std::cout << "Loaded dt: " << conf.settings.dt << "\n";
  

  Body earth {0, 5.972e24, {0, 0}, {0, 0} };
  Body moon {1, 7.348e21, {3.844e8, 0}, {0, 1022} };
  std::vector<Body> bodies {earth, moon};
  std::cout << "initial-positions: 0[" << earth.position[0] << ", " << earth.position[1] << "], 1[" << moon.position[0] << ", " << moon.position[1] << "].";
  
  for (int i = 0; i < 100; i++) {
    velocity_verlet(bodies);
  }

  return 0;
}

