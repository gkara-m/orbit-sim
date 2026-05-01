#include <nlohmann/json.hpp>

#include "physics.hpp"

#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>

using json = nlohmann::json;


Config startup() {
  std::ifstream file("data/config.json");
  json j {};
  file >> j;
  Config config {};
  from_json(j, config);
  return config;
}

auto start_ui_communicator_thread() {
  std::mutex comm_mutex {};
  auto comm_thread {std::thread([&comm_mutex](){
    
  })};
};

auto main() -> int {
  Config conf { startup() };
  std::cout << "Loaded config.json" << "\n";
  
  start_ui_communicator_thread();
  
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

