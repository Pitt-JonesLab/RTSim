set(DECODER_SOURCES
    ${RTSIM_DIR}/Decoders/DecoderFactory.cpp
    ${RTSIM_DIR}/Decoders/Migrator/Migrator.cpp
    ${RTSIM_DIR}/Decoders/DRCDecoder/DRCDecoder.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${DECODER_SOURCES} PARENT_SCOPE)