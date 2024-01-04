set(MEMORY_SOURCES
    FaultModel.cpp
    Bank/Bank.cpp
    Bank/DRAMBank.cpp
    Interconnect/DRAMInterconnect.cpp
    Interconnect/Interconnect.cpp
    MemoryController/DRAMController.cpp
    MemoryController/MemoryController.cpp
    MemorySystem/DRAMSystem.cpp
    Rank/Rank.cpp
    Rank/DRAMRank.cpp
    SubArray/DRAMSubArray.cpp)

add_library(RTSim_MemoryType_DRAM ${MEMORY_SOURCES})
target_include_directories(RTSim_MemoryType_DRAM PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
target_link_libraries(RTSim_MemoryType_DRAM PUBLIC RTSim_Logging RTSim_Modeling RTSim_Scheduling RTSim_Parsing RTSim_Timing RTSim_Stats)