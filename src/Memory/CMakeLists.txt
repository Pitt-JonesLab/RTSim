set(MEMORY_SOURCES
    MemoryFactory.cpp
    StatBlock.cpp
    Instruction.cpp
    Decoder.cpp
    Command/TimedCommand.cpp
    Command/WaitingCommand.cpp
    Command/ChainedCommand.cpp
    Command/NullCommand.cpp
    Command/TriggeredCommand.cpp
    Bank/Bank.cpp
    Bank/SimpleBank.cpp
    Interconnect/SimpleInterconnect.cpp
    Interconnect/Interconnect.cpp
    MemoryController/SimpleController.cpp
    MemoryController/MemoryController.cpp
    MemoryController/InstructionTranslator.cpp
    MemorySystem/SimpleSystem.cpp
    Rank/Rank.cpp
    Rank/SimpleRank.cpp
    SubArray/SimpleSubArray.cpp
    SubArray/RowController.cpp)

add_library(RTSim_Memory ${MEMORY_SOURCES})
target_include_directories(RTSim_Memory PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
target_link_libraries(RTSim_Memory PUBLIC RTSim_Logging)