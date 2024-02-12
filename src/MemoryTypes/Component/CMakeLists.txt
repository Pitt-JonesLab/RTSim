set(COMPONENT_SOURCES
    Bank/Bank.cpp
    Bank/BankCommand.cpp
    Bank/BankResponse.cpp
    Bank/BankState.cpp
    Bank/ClosedState.cpp
    Bank/OpenState.cpp
    Bank/ReadingState.cpp
    Bank/WritingState.cpp
    Controller/BankModel.cpp
    Controller/Parser.cpp
    Controller/Scheduler.cpp
    Controller/MemoryController.cpp
    ComponentSystem.cpp
    Bus.cpp
    MemoryCommand.cpp
    SimpleBankTimingRules.cpp)

add_library(RTSim_MemoryType_Component ${COMPONENT_SOURCES})
target_include_directories(RTSim_MemoryType_Component PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
# target_link_libraries(RTSim_MemoryType_COMPONENT PUBLIC RTSim_MemoryType_Simple)