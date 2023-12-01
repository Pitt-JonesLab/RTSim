set(DECODER_SOURCES
    ${RTSIM_DIR}/src/old/Decoders/DecoderFactory.cpp
    ${RTSIM_DIR}/src/old/Decoders/Migrator/Migrator.cpp
    ${RTSIM_DIR}/src/old/Decoders/DRCDecoder/DRCDecoder.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${DECODER_SOURCES} PARENT_SCOPE)