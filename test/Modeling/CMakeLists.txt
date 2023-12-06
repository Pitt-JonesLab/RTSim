set(TEST_SOURCES
    SubArray/SimpleSubArray.test.cpp
    Decoder.test.cpp)

add_catch_tests(TARGETS ${TEST_SOURCES} LIBS RTSim_Modeling)