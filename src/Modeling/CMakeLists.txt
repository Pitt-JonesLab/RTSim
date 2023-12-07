set(MODELING_SOURCES
    SubArray/SimpleSubArray.cpp
    Bank/SimpleBank.cpp
    Rank/SimpleRank.cpp
    System/SimpleSystem.cpp
    Decoder.cpp)

add_library(RTSim_Modeling ${MODELING_SOURCES})
target_include_directories(RTSim_Modeling PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
target_link_libraries(RTSim_Modeling PUBLIC RTSim_Logging)