set(STATS_SOURCES
    StatBlock.cpp)

add_library(RTSim_Stats ${STATS_SOURCES})
target_include_directories(RTSim_Stats PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)