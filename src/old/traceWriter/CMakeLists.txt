set(TRACEWRITER_SOURCES
    ${RTSIM_DIR}/src/old/traceWriter/GenericTraceWriter.cpp
    ${RTSIM_DIR}/src/old/traceWriter/TraceWriterFactory.cpp
    ${RTSIM_DIR}/src/old/traceWriter/DRAMPower2Trace/DRAMPower2TraceWriter.cpp
    ${RTSIM_DIR}/src/old/traceWriter/NVMainTrace/NVMainTraceWriter.cpp
    ${RTSIM_DIR}/src/old/traceWriter/VerilogTrace/VerilogTraceWriter.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${TRACEWRITER_SOURCES} PARENT_SCOPE)