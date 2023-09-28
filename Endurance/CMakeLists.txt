set(ENDURANCE_SOURCES
    ${RTSIM_DIR}/Endurance/EnduranceDistributionFactory.cpp
    ${RTSIM_DIR}/Endurance/EnduranceModelFactory.cpp
    ${RTSIM_DIR}/Endurance/BitModel/BitModel.cpp
    ${RTSIM_DIR}/Endurance/ByteModel/ByteModel.cpp
    ${RTSIM_DIR}/Endurance/Distributions/Normal.cpp
    ${RTSIM_DIR}/Endurance/Distributions/Uniform.cpp
    ${RTSIM_DIR}/Endurance/NullModel/NullModel.cpp
    ${RTSIM_DIR}/Endurance/RowModel/RowModel.cpp
    ${RTSIM_DIR}/Endurance/WordModel/WordModel.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${ENDURANCE_SOURCES} PARENT_SCOPE)