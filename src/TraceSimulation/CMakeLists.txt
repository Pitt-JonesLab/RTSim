set(SIM_SOURCES
    ${RTSIM_DIR}/src/TraceSimulation/CycleTimer.cpp
    ${RTSIM_DIR}/src/TraceSimulation/TraceIssuer.cpp
    ${RTSIM_DIR}/src/TraceSimulation/TraceRequestMaker.cpp
    ${RTSIM_DIR}/src/TraceSimulation/TraceSimulator.cpp
    ${RTSIM_DIR}/src/TraceSimulation/FileTraceReader.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${SIM_SOURCES} PARENT_SCOPE)