set(TRACEWRITER_SOURCES
    ${RTSIM_DIR}/traceWriter/GenericTraceWriter.cpp
    ${RTSIM_DIR}/traceWriter/TraceWriterFactory.cpp
    ${RTSIM_DIR}/traceWriter/DRAMPower2Trace/DRAMPower2TraceWriter.cpp
    ${RTSIM_DIR}/traceWriter/NVMainTrace/NVMainTraceWriter.cpp
    ${RTSIM_DIR}/traceWriter/VerilogTrace/VerilogTraceWriter.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${TRACEWRITER_SOURCES} PARENT_SCOPE)