set(TEST_SOURCES
    LogStream.test.cpp)

add_catch_tests(TARGETS ${TEST_SOURCES} LIBS RTSim_Logging)