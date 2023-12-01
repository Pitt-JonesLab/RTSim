set(ENDURANCE_SOURCES
    ${RTSIM_DIR}/src/old/Endurance/EnduranceDistributionFactory.cpp
    ${RTSIM_DIR}/src/old/Endurance/EnduranceModelFactory.cpp
    ${RTSIM_DIR}/src/old/Endurance/BitModel/BitModel.cpp
    ${RTSIM_DIR}/src/old/Endurance/ByteModel/ByteModel.cpp
    ${RTSIM_DIR}/src/old/Endurance/Distributions/Normal.cpp
    ${RTSIM_DIR}/src/old/Endurance/Distributions/Uniform.cpp
    ${RTSIM_DIR}/src/old/Endurance/NullModel/NullModel.cpp
    ${RTSIM_DIR}/src/old/Endurance/RowModel/RowModel.cpp
    ${RTSIM_DIR}/src/old/Endurance/WordModel/WordModel.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${ENDURANCE_SOURCES} PARENT_SCOPE)