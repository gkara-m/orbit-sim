install(
    TARGETS orbital_sim_exe
    RUNTIME COMPONENT orbital_sim_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
