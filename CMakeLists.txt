cmake_minimum_required(VERSION 3.10)

project(RTSim)

set(buildtype "debug" CACHE STRING "Build type for RTSim")

set(RTSIM_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RTSIM_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${RTSIM_DIR}/bin/debug)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RTSIM_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${RTSIM_DIR}/lib/debug)

set(CMAKE_CXX_STANDARD 17)

# Lib dirs
add_subdirectory(src)
add_subdirectory(Utils)
add_subdirectory(traceWriter)
add_subdirectory(traceReader)
add_subdirectory(Ranks)
add_subdirectory(Prefetchers)
add_subdirectory(MemControl)
add_subdirectory(Banks)
add_subdirectory(DataEncoders)
add_subdirectory(Decoders)
add_subdirectory(Endurance)
add_subdirectory(FaultModels)
add_subdirectory(Interconnect)
add_subdirectory(include)
add_subdirectory(NVM)
add_subdirectory(SimInterface)

add_library(RTSim ${ALL_SOURCES})
target_include_directories(RTSim PUBLIC ${RTSIM_DIR})

# Main dir
add_subdirectory(traceSim)

# If this is the top-level project, build the tests
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	add_subdirectory(test)
endif()