set(Utils_SOURCES
    Address.cpp
    RowData.cpp)

add_library(RTSim_Utils ${Utils_SOURCES})
target_include_directories(RTSim_Utils PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
target_link_libraries(RTSim_Utils PUBLIC RTSim_Logging)