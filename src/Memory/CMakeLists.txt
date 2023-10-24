set(MEMORY_SOURCES
    MemoryFactory.cpp
    StatBlock.cpp
    TimedCommand.cpp
    WaitingCommand.cpp
    ChainedCommand.cpp
    Bank/SimpleBank.cpp
    Interconnect/SimpleInterconnect.cpp
    MemoryController/SimpleController.cpp
    MemorySystem/SimpleSystem.cpp
    Rank/SimpleRank.cpp
    SubArray/SimpleSubArray.cpp
    SubArray/RowController.cpp)

add_library(RTSim_Memory ${MEMORY_SOURCES})
target_include_directories(RTSim_Memory PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
target_link_libraries(RTSim_Memory PUBLIC RTSim_Logging)