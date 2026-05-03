#include "physics.hpp"

#include <cstddef>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

auto sim_loop(std::shared_ptr<SharedState> shared_state, size_t num_bodies) {
  // run the physics sim loop with a unique lock
  {
    std::unique_lock lock(shared_state->mutex);
    velocity_verlet(shared_state->state, num_bodies);
  };

  std::this_thread::yield();
}

auto sim_entry(std::shared_ptr<SharedState> shared_state, Settings settings) -> int {
  size_t num_bodies {0};
  // lock shared_state to read num_bodies
  {
    std::shared_lock lock(shared_state->mutex);
    num_bodies += shared_state->state.masses.size();
  };

  if (settings.use_steps == false) {
    while (true) {
      sim_loop(shared_state, num_bodies);
    }
  }
  for (int i = 0; i < settings.steps; i++) {
    sim_loop(shared_state, num_bodies);
  }

  return 0;
}
