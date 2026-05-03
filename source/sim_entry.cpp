#include "physics.hpp"

#include <memory>

auto sim_entry(std::shared_ptr<SharedState> shared_state, Settings settings) -> int {
  size_t num_bodies {shared_state->state.masses.size()};
  if (num_bodies == 0) return 1;

  if (settings.use_steps == false) {
    while (shared_state->keep_running) {
      velocity_verlet(shared_state->state, num_bodies);
    };
    return 0;

  } else {for (int i {0}; i < settings.steps; i++) {
      velocity_verlet(shared_state->state, num_bodies);
    };
    return 0;

  };
}
