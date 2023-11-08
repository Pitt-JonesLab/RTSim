set(TEST_SOURCES
    CycleTimer.test.cpp
    TraceReader.test.cpp
    TraceIssuer.test.cpp
    TraceSimulator.test.cpp
    ReadCommand.test.cpp
    WriteCommand.test.cpp
    Config.test.cpp)

add_catch_tests(TARGETS ${TEST_SOURCES} LIBS RTSim_Simulation)