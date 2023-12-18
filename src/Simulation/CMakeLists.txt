set(SIMULATION_SOURCES
    CycleTimer.cpp
    TraceIssuer.cpp
    TraceSimulator.cpp
    ${RTSIM_DIR}/src/old/NVMDataBlock.cpp
    Config.cpp)

add_subdirectory(TraceReader)

add_library(RTSim_Simulation ${SIMULATION_SOURCES})
target_include_directories(RTSim_Simulation PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src ${RTSIM_DIR})
target_link_libraries(RTSim_Simulation RTSim_Logging RTSim_Utils RTSim_Memory)