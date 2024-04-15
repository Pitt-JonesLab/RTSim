set(MEMORY_SOURCES
    MemoryFactory.cpp)

add_library(RTSim_Memory ${MEMORY_SOURCES})
target_include_directories(RTSim_Memory PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
target_link_libraries(RTSim_Memory PUBLIC RTSim_MemoryType_Simple RTSim_MemoryType_State RTSim_MemoryType_Component)