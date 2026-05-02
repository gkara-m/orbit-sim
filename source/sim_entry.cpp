#include "physics.hpp"
#include "sim_entry.hpp"

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

// TODO
auto ui_send_data() {}
auto ui_recv_data() {}

auto start_ui_communicator_thread(const Config conf) {
  std::mutex comm_mutex {};
  auto comm_thread {std::thread([&comm_mutex, conf](){
    while (true) {
      if (comm_mutex.try_lock() == true) {
        int error_code {
          // TODO
        };
        if (error_code != 0) {return error_code;};
      };
      comm_mutex.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
    };
  })};
  comm_thread.detach();
}; // TODO

auto sim_entry(Config conf) -> int {
  std::cout << "Loaded config.json" << "\n";
  State state { conf.state };
  
  // start_ui_communicator_thread(conf);
  
  std::cout << "Initial Positions: ";
  size_t num_bodies {state.masses.size()};
  for (size_t i {0}; i < num_bodies; ++i) {
    std::cout << "[" << state.positions[2*i] << "," << state.positions[2*i+1] << "] ";
  };

  for (int i = 0; i < conf.settings.steps; i++) {

    velocity_verlet(state, num_bodies);

    for (size_t i {0}; i < num_bodies; ++i) {
      std::cout << "[" << state.positions[2*i] << "," << state.positions[2*i+1] << "] ";
    };
    std::cout << "\n";
  }

  return 0;
}
