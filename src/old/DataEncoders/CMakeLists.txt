set(ENCODER_SOURCES
    ${RTSIM_DIR}/src/old/DataEncoders/DataEncoderFactory.cpp
    ${RTSIM_DIR}/src/old/DataEncoders/FlipNWrite/FlipNWrite.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${ENCODER_SOURCES} PARENT_SCOPE)