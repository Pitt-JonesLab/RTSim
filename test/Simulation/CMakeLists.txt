set(TEST_SOURCES
    CycleTimer.test.cpp
    TraceReader.test.cpp
    TraceSimulator.test.cpp
    Config.test.cpp)

add_catch_tests(TARGETS ${TEST_SOURCES} LIBS RTSim_Simulation)