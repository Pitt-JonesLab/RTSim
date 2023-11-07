set(TEST_SOURCES
    MemoryFactory.test.cpp
    SimpleSystem.test.cpp
    SimpleController.test.cpp
    SimpleInterconnect.test.cpp
    SimpleSubArray.test.cpp
    TimedCommand.test.cpp
    WaitingCommand.test.cpp
    ChainedCommand.test.cpp
    Decoder.test.cpp)

add_catch_tests(TARGETS ${TEST_SOURCES} LIBS RTSim_Memory RTSim_Logging RTSim_Simulation)
