set(PARSING_SOURCES
    Command.cpp
    Parser/SimpleParser.cpp)

add_library(RTSim_Parsing ${PARSING_SOURCES})
target_include_directories(RTSim_Parsing PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)
target_link_libraries(RTSim_Parsing PUBLIC RTSim_Modeling)