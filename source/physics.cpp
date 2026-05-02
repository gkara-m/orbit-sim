#include "physics.hpp"

#include <cstddef>
#include <vector>
#include <cmath>

void vv_update_pos(double& pos, const double vel, const double acc, const double dt) {
  pos+= (vel * dt + acc * dt * dt / 2);
}

void velocity_verlet(State& state, const size_t num_bodies) {
  double dt {state.physics.dt};
  double G {state.physics.g};

  // *2 as 2d vectors
  for (size_t i {0}; i < num_bodies * 2; ++i) {
    vv_update_pos(state.positions[i], state.velocities[i], state.accelerations[i], dt);
  };

  // loop across each body to update vel and acc
  for (size_t i {0}; i < num_bodies; ++i) {
    double acc_x_new {0};
    double acc_y_new {0};

    // sum acceleration on i from each body, add to existing acc
    for (size_t j {0}; j < num_bodies; ++j) {
      if (i == j) continue;

      // i -> j
      double dx = state.positions[j * 2] - state.positions[i * 2];
      double dy = state.positions[j * 2 + 1] - state.positions[i * 2 + 1];
      double dist_sq = dx * dx + dy * dy;
      double dist = std::sqrt(dist_sq);

      // avoid div by 0
      if (dist < 1.0) continue; 

      double force_scalar = (G * state.masses[j]) / dist_sq;
      acc_x_new += force_scalar * (dx / dist);
      acc_y_new += force_scalar * (dy / dist);
    };

    state.velocities[i * 2] += (state.accelerations[i * 2] + acc_x_new) * dt / 2;
    state.velocities[i * 2 + 1] += (state.accelerations[i * 2 + 1] + acc_y_new) * dt / 2;
    state.accelerations[i * 2] = acc_x_new;
    state.accelerations[i * 2 + 1] = acc_y_new;
  };

  return;
}
