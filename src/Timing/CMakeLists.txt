set(TIMING_SOURCES
    Timer/SimpleTimer.cpp
    Timer/ConfigurableTimer.cpp)

add_library(RTSim_Timing ${TIMING_SOURCES})
target_include_directories(RTSim_Timing PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
target_link_libraries(RTSim_Timing PUBLIC RTSim_Modeling)