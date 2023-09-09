set(MEMCONTROL_SOURCES
    ${RTSIM_DIR}/MemControl/MemoryControllerFactory.cpp
    ${RTSIM_DIR}/MemControl/DRAMCache/DRAMCache.cpp
    ${RTSIM_DIR}/MemControl/FCFS/FCFS.cpp
    ${RTSIM_DIR}/MemControl/FRFCFS/FRFCFS.cpp
    ${RTSIM_DIR}/MemControl/FRFCFS-WQF/FRFCFS-WQF.cpp
    ${RTSIM_DIR}/MemControl/LH-Cache/LH-Cache.cpp
    ${RTSIM_DIR}/MemControl/LO-Cache/LO-Cache.cpp
    ${RTSIM_DIR}/MemControl/MissMap/MissMap.cpp
    ${RTSIM_DIR}/MemControl/PerfectMemory/PerfectMemory.cpp
    ${RTSIM_DIR}/MemControl/PredictorDRC/PredictorDRC.cpp
    ${RTSIM_DIR}/MemControl/RTM/RTM.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${MEMCONTROL_SOURCES} PARENT_SCOPE)