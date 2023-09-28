set(INTERCONNECT_SOURCES
    ${RTSIM_DIR}/Interconnect/InterconnectFactory.cpp
    ${RTSIM_DIR}/Interconnect/OffChipBus/OffChipBus.cpp
    ${RTSIM_DIR}/Interconnect/OnChipBus/OnChipBus.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${INTERCONNECT_SOURCES} PARENT_SCOPE)