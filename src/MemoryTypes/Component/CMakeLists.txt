set(COMPONENT_SOURCES
    ComponentSystem.cpp
    Bank.cpp
    Bus.cpp
    MemoryController.cpp
    MemoryCommand.cpp
    BankResponse.cpp)

add_library(RTSim_MemoryType_Component ${COMPONENT_SOURCES})
target_include_directories(RTSim_MemoryType_Component PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
# target_link_libraries(RTSim_MemoryType_COMPONENT PUBLIC RTSim_MemoryType_Simple)