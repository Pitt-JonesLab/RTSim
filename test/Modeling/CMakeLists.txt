set(TEST_SOURCES
    SubArray/SimpleSubArray.test.cpp
    Bank/SimpleBank.test.cpp
    Rank/SimpleRank.test.cpp
    System/SimpleSystem.test.cpp
    Decoder.test.cpp)

add_catch_tests(TARGETS ${TEST_SOURCES} LIBS RTSim_Modeling)