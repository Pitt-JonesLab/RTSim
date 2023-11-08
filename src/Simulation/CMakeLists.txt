set(SIMULATION_SOURCES
    CycleTimer.cpp
    TraceIssuer.cpp
    TraceSimulator.cpp
    TraceReader.cpp
    FileTraceReader.cpp
    ReadCommand.cpp
    WriteCommand.cpp
    TransverseReadCommand.cpp
    TransverseWriteCommand.cpp
    RowCloneCommand.cpp
    ${RTSIM_DIR}/include/NVMDataBlock.cpp
    Config.cpp)

add_library(RTSim_Simulation ${SIMULATION_SOURCES})
target_include_directories(RTSim_Simulation PUBLIC ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src ${RTSIM_DIR}) 