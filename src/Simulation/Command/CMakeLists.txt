set(COMMAND_SOURCES
    Command/ReadCommand.cpp
    Command/RowCloneCommand.cpp
    Command/TransverseReadCommand.cpp
    Command/TransverseWriteCommand.cpp
    Command/WriteCommand.cpp
    Command/ECCCommand.cpp)

set(SIMULATION_SOURCES ${SIMULATION_SOURCES} ${COMMAND_SOURCES} PARENT_SCOPE)