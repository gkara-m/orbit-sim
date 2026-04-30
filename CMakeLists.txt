cmake_minimum_required(VERSION 3.24)

include(cmake/prelude.cmake)

project(
    orbital_sim
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)
include(FetchContent)

FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(JSON_BuildTests OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(json)

# ---- Declare library ----

add_library(
    orbital_sim_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    orbital_sim_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(orbital_sim_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

add_executable(orbital_sim_exe source/main.cpp)
add_executable(orbital_sim::exe ALIAS orbital_sim_exe)

set_property(TARGET orbital_sim_exe PROPERTY OUTPUT_NAME orbital_sim)

target_compile_features(orbital_sim_exe PRIVATE cxx_std_20)

target_link_libraries(orbital_sim_exe PRIVATE orbital_sim_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT orbital_sim_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of orbital_sim"
  )
endif()

include(cmake/dev-mode.cmake)
