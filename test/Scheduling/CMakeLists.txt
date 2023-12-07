set(TEST_SOURCES
    Scheduler/FIFOScheduler.test.cpp)

add_catch_tests(TARGETS ${TEST_SOURCES} LIBS RTSim_Scheduling)