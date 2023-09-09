set(BANK_SOURCES
    ${RTSIM_DIR}/Banks/BankFactory.cpp
    ${RTSIM_DIR}/Banks/CachedDDR3Bank/CachedDDR3Bank.cpp
    ${RTSIM_DIR}/Banks/DDR3Bank/DDR3Bank.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${BANK_SOURCES} PARENT_SCOPE)