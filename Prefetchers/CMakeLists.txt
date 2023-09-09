set(PREFETCHER_SOURCES
    ${RTSIM_DIR}/Prefetchers/PrefetcherFactory.cpp
    ${RTSIM_DIR}/Prefetchers/NaivePrefetcher/NaivePrefetcher.cpp
    ${RTSIM_DIR}/Prefetchers/STeMS/STeMS.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${PREFETCHER_SOURCES} PARENT_SCOPE)