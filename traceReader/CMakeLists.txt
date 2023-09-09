set(TRACEREADER_SOURCES
    ${RTSIM_DIR}/traceReader/TraceLine.cpp
    ${RTSIM_DIR}/traceReader/TraceReaderFactory.cpp
    ${RTSIM_DIR}/traceReader/NVMainTrace/NVMainTraceReader.cpp
    ${RTSIM_DIR}/traceReader/RubyTrace/RubyTraceReader.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${TRACEREADER_SOURCES} PARENT_SCOPE)