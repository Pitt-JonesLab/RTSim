set(MEMORY_SOURCES
    Bank/Bank.cpp
    Bank/SimpleBank.cpp
    Interconnect/SimpleInterconnect.cpp
    Interconnect/Interconnect.cpp
    MemoryController/SimpleController.cpp
    MemoryController/MemoryController.cpp
    MemorySystem/SimpleSystem.cpp
    Rank/Rank.cpp
    Rank/SimpleRank.cpp
    SubArray/SimpleSubArray.cpp)

add_library(RTSim_MemoryType_Simple ${MEMORY_SOURCES})
target_include_directories(RTSim_MemoryType_Simple PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
target_link_libraries(RTSim_MemoryType_Simple PUBLIC RTSim_Logging RTSim_Modeling RTSim_Scheduling RTSim_Parsing RTSim_Timing RTSim_Stats)