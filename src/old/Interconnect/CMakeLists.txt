set(INTERCONNECT_SOURCES
    ${RTSIM_DIR}/src/old/Interconnect/InterconnectFactory.cpp
    ${RTSIM_DIR}/src/old/Interconnect/OffChipBus/OffChipBus.cpp
    ${RTSIM_DIR}/src/old/Interconnect/OnChipBus/OnChipBus.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${INTERCONNECT_SOURCES} PARENT_SCOPE)