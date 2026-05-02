#include "physics.hpp"

#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>


auto sim_entry(std::shared_ptr<SharedState> shared_state, Settings settings) -> int {
  std::cout << "Loaded config.json" << "\n";


  std::cout << "Initial Positions: ";
  size_t num_bodies {0};
  {
    std::shared_lock lock(shared_state->mutex);
    num_bodies += shared_state->state.masses.size();
    for (size_t i {0}; i < num_bodies; ++i) {
      std::shared_lock lock(shared_state->mutex);
      std::vector<double> positions {shared_state->state.positions};
      std::cout << "[" << positions[2*i] << "," << positions[2*i+1] << "] ";
    };
  }

  for (int i = 0; i < settings.steps; i++) {
    {
      std::unique_lock lock(shared_state->mutex);
      velocity_verlet(shared_state->state, num_bodies);
    };

    {
      std::shared_lock lock(shared_state->mutex);
      std::vector<double> positions {shared_state->state.positions};
      for (size_t i {0}; i < num_bodies; ++i) {
        std::cout << "[" << positions[2*i] << "," << positions[2*i+1] << "] ";
      };
    };
    std::cout << "\n";

    std::this_thread::yield();
  }

  return 0;
}
