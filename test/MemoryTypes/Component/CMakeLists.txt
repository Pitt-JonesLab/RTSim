set(TEST_SOURCES
    Bank.test.cpp)

add_catch_tests(TARGETS ${TEST_SOURCES} LIBS RTSim_MemoryType_Component RTSim_Utils RTSim_Stats RTSim_Simulation RTSim_Modeling)