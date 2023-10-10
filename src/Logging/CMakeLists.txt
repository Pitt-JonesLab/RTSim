SET(LOGGING_SOURCES
    Logging.cpp
    LogStream.cpp)

add_library(RTSim_Logging ${LOGGING_SOURCES})
target_include_directories(RTSim_Logging PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)