set(EXE_NAME rtsim.${buildtype})
add_executable(${EXE_NAME} ${RTSIM_DIR}/src/main/simpleMain.cpp)
target_link_libraries(${EXE_NAME} PRIVATE RTSim)
target_include_directories(${EXE_NAME} PUBLIC ${RTSIM_DIR} ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src)