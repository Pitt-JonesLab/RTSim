set(TRACEMAIN_SOURCES
    ${RTSIM_DIR}/src/main/RTSystem.cpp
    ${RTSIM_DIR}/src/main/traceMain.cpp)

set(EXE_NAME nvmain.test.${buildtype})
add_executable(${EXE_NAME} ${TRACEMAIN_SOURCES})
target_link_libraries(${EXE_NAME} PRIVATE RTSim)
target_include_directories(${EXE_NAME} PRIVATE ${RTSIM_DIR})