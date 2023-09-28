set(RANK_SOURCES
    ${RTSIM_DIR}/Ranks/RankFactory.cpp
    ${RTSIM_DIR}/Ranks/StandardRank/StandardRank.cpp)

set(ALL_SOURCES ${ALL_SOURCES} ${RANK_SOURCES} PARENT_SCOPE)