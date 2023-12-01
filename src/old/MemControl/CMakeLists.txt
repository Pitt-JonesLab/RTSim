set(MEMCONTROL_SOURCES
    ${RTSIM_DIR}/src/old/MemControl/MemoryControllerFactory.cpp
    ${RTSIM_DIR}/src/old/MemControl/DRAMCache/DRAMCache.cpp
    ${RTSIM_DIR}/src/old/MemControl/FCFS/FCFS.cpp
    ${RTSIM_DIR}/src/old/MemControl/FRFCFS/FRFCFS.cpp
    ${RTSIM_DIR}/src/old/MemControl/FRFCFS-WQF/FRFCFS-WQF.cpp
    ${RTSIM_DIR}/src/old/MemControl/LH-Cache/LH-Cache.cpp
    ${RTSIM_DIR}/src/old/MemControl/LO-Cache/LO-Cache.cpp
    ${RTSIM_DIR}/src/old/MemControl/MissMap/MissMap.cpp
    ${RTSIM_DIR}/src/old/MemControl/PerfectMemory/PerfectMemory.cpp
    ${RTSIM_DIR}/src/old/MemControl/PredictorDRC/PredictorDRC.cpp
    ${RTSIM_DIR}/src/old/MemControl/RTM/RTM.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${MEMCONTROL_SOURCES} PARENT_SCOPE)