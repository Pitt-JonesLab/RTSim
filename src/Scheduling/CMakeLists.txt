set(SCHEDULING_SOURCES
    Instruction.cpp
    Scheduler/FIFOScheduler.cpp
    Scheduler/RBScheduler.cpp)

add_library(RTSim_Scheduling ${SCHEDULING_SOURCES})
target_include_directories(RTSim_Scheduling PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
target_link_libraries(RTSim_Scheduling PUBLIC RTSim_Modeling)