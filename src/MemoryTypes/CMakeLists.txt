set(MEMORY_SOURCES
    MemoryController.cpp
    Interconnect.cpp
    Rank.cpp 
    Bank.cpp )

add_subdirectory(Simple)
add_subdirectory(DRAM)

add_library(RTSim_MemoryType ${MEMORY_SOURCES})
target_include_directories(RTSim_MemoryType PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
target_link_libraries(RTSim_MemoryType PUBLIC RTSim_MemoryType_Simple RTSim_Logging RTSim_Modeling RTSim_Scheduling RTSim_Parsing RTSim_Timing RTSim_Stats)