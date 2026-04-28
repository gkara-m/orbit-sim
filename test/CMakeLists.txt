# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(orbital_simTests LANGUAGES CXX)

# ---- Tests ----

add_executable(orbital_sim_test source/orbital_sim_test.cpp)
target_link_libraries(orbital_sim_test PRIVATE orbital_sim_lib)
target_compile_features(orbital_sim_test PRIVATE cxx_std_20)

add_test(NAME orbital_sim_test COMMAND orbital_sim_test)

# ---- End-of-file commands ----

add_folders(Test)
