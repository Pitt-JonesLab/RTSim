cmake_minimum_required(VERSION 3.10)

project(RTSim)

set(buildtype "debug" CACHE STRING "Build type for RTSim")
set(coverage FALSE CACHE BOOL "Whether to track code coverage")

set(RTSIM_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RTSIM_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${RTSIM_DIR}/bin/debug)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RTSIM_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${RTSIM_DIR}/lib/debug)

set(CMAKE_CXX_STANDARD 17)

set(CXXFLAGS "-O0 -g")

if(coverage)
	set(CXXFLAGS "${CXXFLAGS} --coverage")
endif()

set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

# Lib dirs
add_subdirectory(src)

add_library(RTSim ${ALL_SOURCES})
target_include_directories(RTSim PUBLIC ${RTSIM_DIR} ${RTSIM_DIR}/include PRIVATE ${RTSIM_DIR}/src ${RTSIM_DIR}/src/old)
target_link_libraries(RTSim PUBLIC RTSim_Simulation RTSim_Logging RTSim_Memory)

# If this is the top-level project, build the tests
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	add_subdirectory(test)
endif()