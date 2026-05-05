#include "velocity_verlet.hpp"
#include "sim_entry.hpp"

auto sim_entry(std::shared_ptr<SharedState> shared_state, Settings settings) -> int {
  size_t num_bodies {shared_state->state.masses.size()};
  if (num_bodies == 0) return 1;
  if (settings.integrator != 0) return 1;

  if (settings.algorithm == 0) {
    if (settings.use_steps == false) {
      while (shared_state->keep_running) {
        vv_brute_force(shared_state->state, num_bodies);
      };
      return 0;

    } else {for (int i {0}; i < settings.steps; i++) {
        vv_brute_force(shared_state->state, num_bodies);
      };
      return 0;
    };
  };

  if (settings.algorithm == 1) {
    if (settings.use_steps == false) {
      while (shared_state->keep_running) {
        vv_barnes_hut(shared_state->state, num_bodies, settings.bh_theta);
      };
      return 0;

    } else {for (int i {0}; i < settings.steps; i++) {
        vv_barnes_hut(shared_state->state, num_bodies, settings.bh_theta);
      };
      return 0;
    };
  };

  return 1;
}
