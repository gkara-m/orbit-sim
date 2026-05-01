cmake_minimum_required(VERSION 3.24)
project(orbital_sim LANGUAGES CXX)

find_package(nlohmann_json REQUIRED)

add_executable(orbital_sim
  source/main.cpp
  source/physics.cpp
  source/math_utils.cpp
  source/types.cpp
)
target_link_libraries(orbital_sim PRIVATE nlohmann_json::nlohmann_json)
