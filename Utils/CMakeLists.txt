set(UTIL_SOURCES
    ${RTSIM_DIR}/Utils/HookFactory.cpp
    ${RTSIM_DIR}/Utils/AccessPredictor/AccessPredictor.cpp
    ${RTSIM_DIR}/Utils/AccessPredictor/AccessPredictorFactory.cpp
    ${RTSIM_DIR}/Utils/AccessPredictor/PerfectPredictor/PerfectPredictor.cpp
    ${RTSIM_DIR}/Utils/AccessPredictor/VariablePredictor/VariablePredictor.cpp
    ${RTSIM_DIR}/Utils/Caches/CacheBank.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${UTIL_SOURCES} PARENT_SCOPE)