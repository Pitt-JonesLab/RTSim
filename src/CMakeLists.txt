set(MAIN_SOURCES
    ${RTSIM_DIR}/src/AddressTranslator.cpp 
    ${RTSIM_DIR}/src/Bank.cpp
    ${RTSIM_DIR}/src/Config.cpp 
    ${RTSIM_DIR}/src/DataEncoder.cpp 
    ${RTSIM_DIR}/src/Debug.cpp 
    ${RTSIM_DIR}/src/EnduranceModel.cpp 
    ${RTSIM_DIR}/src/EventQueue.cpp 
    ${RTSIM_DIR}/src/FaultModel.cpp 
    ${RTSIM_DIR}/src/Interconnect.cpp 
    ${RTSIM_DIR}/src/MemoryController.cpp 
    ${RTSIM_DIR}/src/NVMObject.cpp 
    ${RTSIM_DIR}/src/Params.cpp 
    ${RTSIM_DIR}/src/Prefetcher.cpp 
    ${RTSIM_DIR}/src/Rank.cpp  
    ${RTSIM_DIR}/src/SimInterface.cpp 
    ${RTSIM_DIR}/src/Stats.cpp  
    ${RTSIM_DIR}/src/SubArray.cpp 
    ${RTSIM_DIR}/src/TagGenerator.cpp 
    ${RTSIM_DIR}/src/TranslationMethod.cpp
    ${RTSIM_DIR}/src/SubArrayCounter.cpp
    ${RTSIM_DIR}/src/ModuleTiming.cpp
    ${RTSIM_DIR}/src/RequestMaker.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${MAIN_SOURCES} PARENT_SCOPE)