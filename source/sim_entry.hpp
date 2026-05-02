#pragma once

#include "types.hpp"
#include <memory>
int sim_entry(std::shared_ptr<SharedState> shared, Settings settings);
