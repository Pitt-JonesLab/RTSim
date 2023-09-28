set(TRACEMAIN_SOURCES
    traceMain.cpp)

set(EXE_NAME nvmain.${buildtype})
add_executable(${EXE_NAME} ${TRACEMAIN_SOURCES})
target_link_libraries(${EXE_NAME} PRIVATE RTSim)
target_include_directories(${EXE_NAME} PRIVATE ${RTSIM_DIR})