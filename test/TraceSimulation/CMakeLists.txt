set(TEST_SOURCES
    CycleTimer.test.cpp
    TraceIssuer.test.cpp
    TraceRequestMaker.test.cpp
    TraceSimulator.test.cpp)

add_catch_tests(TARGETS ${TEST_SOURCES} LIBS RTSim)