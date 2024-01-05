set(PREFETCHER_SOURCES
    ${RTSIM_DIR}/src/old/Prefetchers/PrefetcherFactory.cpp
    ${RTSIM_DIR}/src/old/Prefetchers/NaivePrefetcher/NaivePrefetcher.cpp
    ${RTSIM_DIR}/src/old/Prefetchers/STeMS/STeMS.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${PREFETCHER_SOURCES} PARENT_SCOPE)