set(STATE_SOURCES
    StateSystem.cpp
    StateBank.cpp
    ReadingState.cpp
    WritingState.cpp
    ClosedState.cpp
    OpenState.cpp)

add_library(RTSim_MemoryType_State ${STATE_SOURCES})
target_include_directories(RTSim_MemoryType_State PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
# target_link_libraries(RTSim_MemoryType_State PUBLIC RTSim_MemoryType_Simple)