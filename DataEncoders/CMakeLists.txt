set(ENCODER_SOURCES
    ${RTSIM_DIR}/DataEncoders/DataEncoderFactory.cpp
    ${RTSIM_DIR}/DataEncoders/FlipNWrite/FlipNWrite.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${ENCODER_SOURCES} PARENT_SCOPE)