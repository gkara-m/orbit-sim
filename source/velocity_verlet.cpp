#include "velocity_verlet.hpp"

#include <cmath>

void vv_update_pos(double& pos, const double vel, const double acc, const double dt) {
  pos += (vel * dt + acc * dt * dt / 2);
}

void vv_update_vel(double& vel, const double acc_old, const double acc_new, const double dt) {
  vel += (acc_old + acc_new) * dt / 2;
}

void acceleration_brute_force(const State& state, const int i, const int j, double& acc_x, double& acc_y ) {
  if (i == j) return;

  // i -> j
  double dx = state.positions[j * 2] - state.positions[i * 2];
  double dy = state.positions[j * 2 + 1] - state.positions[i * 2 + 1];
  double dist_sq = dx * dx + dy * dy;
  double dist = std::sqrt(dist_sq);

  // avoid div by 0
  if (dist < 1.0) return; 

  double force_scalar = (state.physics.g * state.masses[j]) / dist_sq;
  acc_x += force_scalar * (dx / dist);
  acc_y += force_scalar * (dy / dist);
}



void velocity_verlet(State& state, const int algorithm, const size_t num_bodies) {
  if (algorithm != 0) return; // TODO
  double dt {state.physics.dt};

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
      acceleration_brute_force(state, i, j, acc_x_new, acc_y_new);
    };
    
    vv_update_vel(state.velocities[2*i], state.accelerations[2*i], acc_x_new, dt);
    vv_update_vel(state.velocities[2*i + 1], state.accelerations[2*i + 1], acc_y_new, dt);
    state.accelerations[i * 2] = acc_x_new;
    state.accelerations[i * 2 + 1] = acc_y_new;
  };

  return;
}
