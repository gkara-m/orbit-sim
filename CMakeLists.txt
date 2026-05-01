cmake_minimum_required(VERSION 3.24)
project(orbital_sim LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json REQUIRED)

execute_process(
    COMMAND node -p "require('node-addon-api').include"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE NODE_ADDON_API_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

add_library(${PROJECT_NAME} MODULE
  source/bridge.cpp
  source/sim_entry.cpp
  source/physics.cpp
  source/math_utils.cpp
  source/types.cpp
)

# treat as SYSTEM headers to remove 3rd party warnings
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${NODE_ADDON_API_DIR}
    ${CMAKE_JS_INC}
)

target_include_directories(${PROJECT_NAME} PRIVATE source/)

set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    SUFFIX ".node"
)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
