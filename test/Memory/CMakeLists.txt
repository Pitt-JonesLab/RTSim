set(TEST_SOURCES
    MemoryFactory.test.cpp
    SimpleSystem.test.cpp
    SimpleController.test.cpp
    SimpleInterconnect.test.cpp
    SimpleRank.test.cpp
    SimpleBank.test.cpp
    SimpleSubArray.test.cpp)

add_catch_tests(TARGETS ${TEST_SOURCES} LIBS RTSim_Memory)