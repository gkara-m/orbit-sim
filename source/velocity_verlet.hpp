#pragma once

#include "types.hpp"

void vv_brute_force(State& state, const size_t num_bodies);
void vv_barnes_hut(State& state, const size_t num_bodies, const double bh_theta);
