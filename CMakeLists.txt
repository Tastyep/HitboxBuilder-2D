cmake_minimum_required (VERSION 2.8.1)
cmake_policy(SET CMP0042 NEW)

set(NAME "HitboxBuilder")
project (${NAME} CXX)

# Global variables
set(MODULE_DIR cmake)
set(SOURCE_DIR src)
set(INCLUDE_DIR include)
set(LIB_DIR lib)
set(TESTBED_DIR example)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

find_package(SFML 2.1 REQUIRED system window graphics audio)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -g")
set(CMAKE_EXE_LINKER_FLAGS  "-g" )

include_directories (${PROJECT_SOURCE_DIR}/${INCLUDE_DIR})

file(GLOB_RECURSE LIB_SOURCE_FILES "${PROJECT_SOURCE_DIR}/${SOURCE_DIR}/*pp")
file(GLOB_RECURSE TESTBED_SOURCE_FILES "${PROJECT_SOURCE_DIR}/${TESTBED_DIR}/*pp")

list(APPEND LIB_SOURCE_FILES ${TESTBED_SOURCE_FILES})
message(STATUS "LIB_SOURCE_FILES: ${LIB_SOURCE_FILES}")

add_executable(${NAME} ${LIB_SOURCE_FILES})

target_link_libraries (${NAME} ${SFML_LIBRARIES})
