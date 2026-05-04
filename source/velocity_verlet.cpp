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

struct Area {
  double centre_x {};
  double centre_y {};
  double size {};

  bool contains(const double x, const double y) {
    if (x >= centre_x - size && x <= centre_x + size && y >= centre_y - size && y <= centre_y + size) return true;
    return false;
  }
};

struct Node {
  Area boundary {};
  int body_index {-1}; // -1 => leaf
  Node* children[4] {nullptr};
  bool divided {false};

  double mass {0};
  double com_x {};
  double com_y {};

  Node(Area b) : boundary(b) {};
  ~Node() {
    for (int i {0}; i < 4; ++i) {
      if (children[i] != nullptr) {
        delete children[i];
      }
    }
  }

  void subdivide() {
    double parent_x {boundary.centre_x};
    double parent_y {boundary.centre_y};
    double child_size { boundary.size / 2 };

    children[0] = new Node {Area {parent_x + child_size, parent_y + child_size, child_size}};
    children[1] = new Node {Area {parent_x - child_size, parent_y + child_size, child_size}};
    children[2] = new Node {Area {parent_x - child_size, parent_y - child_size, child_size}};
    children[3] = new Node {Area {parent_x + child_size, parent_y - child_size, child_size}};
    
    divided = true;
    return;
  }

  void insert(std::vector<double>& positions, int index, int depth = 0) {
    if (depth > 20) return;
    if (!boundary.contains(positions[2*index], positions[2*index + 1])) return;

    // case: empty leaf
    if (!divided && body_index == -1) {
      body_index = index;
      return;
    }

    // case: leaf (full) -> branch
    // divide, turn current leaf into branch, pass down current index to next leaf
    if (!divided) {
      subdivide();

      int old_index {body_index};
      body_index = -1;

      if (old_index != -1) {
        for (int i {0}; i < 4; ++i) {
          children[i]->insert(positions, old_index, depth + 1);
        }
      }
    }

    // case: branch (previous cases return or resolve into branch)
    // pass down new index
    for (int i = 0; i < 4; ++i) {
      children[i]->insert(positions, index, depth + 1);
    }
    
    return;
  }
};

void barnes_hut(State& state, const size_t num_bodies) {
  double max_x {state.positions[0]};
  double min_x {state.positions[0]};
  double max_y {state.positions[1]};
  double min_y {state.positions[1]};

  // get system dimensions
  for (int i {1}; i < num_bodies; ++i) {
    if (state.positions[2*i] > max_x) max_x = state.positions[2*i];
    if (state.positions[2*i] < min_x) min_x = state.positions[2*i];
    if (state.positions[2*i + 1] > max_y) max_y = state.positions[2*i + 1];
    if (state.positions[2*i + 1] < min_y) min_y = state.positions[2*i + 1];
  } // TODO
  
  double width { max_x - min_x };
  double height { max_y - min_y };
  state.system_dimensions = { width, height, (min_x + max_x) / 2, (min_y + max_y) / 2 };
  double size { std::max(width, height) * 1.05};

  // construct quadtree (TODO implement barnes hut)
  Node* root_node = new Node {Area {state.system_dimensions[2], state.system_dimensions[3], size / 2}};
  for (int i {0}; i < num_bodies; ++i) {
    root_node->insert(state.positions, i);
  }

  delete root_node;
};

void velocity_verlet(State& state, const int algorithm, const size_t num_bodies) {
  if (algorithm != 0) return; // TODO
  double dt {state.physics.dt};

  // *2 as 2d vectors
  for (size_t i {0}; i < num_bodies * 2; ++i) {
    vv_update_pos(state.positions[i], state.velocities[i], state.accelerations[i], dt);
  };

  barnes_hut(state, num_bodies);

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
