set(UTIL_SOURCES
    ${RTSIM_DIR}/src/old/Utils/HookFactory.cpp
    ${RTSIM_DIR}/src/old/Utils/AccessPredictor/AccessPredictor.cpp
    ${RTSIM_DIR}/src/old/Utils/AccessPredictor/AccessPredictorFactory.cpp
    ${RTSIM_DIR}/src/old/Utils/AccessPredictor/PerfectPredictor/PerfectPredictor.cpp
    ${RTSIM_DIR}/src/old/Utils/AccessPredictor/VariablePredictor/VariablePredictor.cpp
    ${RTSIM_DIR}/src/old/Utils/Caches/CacheBank.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${UTIL_SOURCES} PARENT_SCOPE)