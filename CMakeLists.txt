cmake_minimum_required (VERSION 2.8.1)
cmake_policy(SET CMP0042 NEW)

set(NAME "HitboxBuilder")
project (${NAME} CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

find_package(SFML 2.1 REQUIRED system window graphics audio)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")

include_directories (${PROJECT_SOURCE_DIR}/inc)

file (GLOB headers "*.h")
file (GLOB sources "*.cpp")

set (DIRS ${PROJECT_SOURCE_DIR}/inc
          ${PROJECT_SOURCE_DIR}/src
          ${PROJECT_SOURCE_DIR}/example)

foreach (dir ${DIRS})
   file (GLOB h_${dir} "${dir}/*.h")
   file (GLOB s_${dir} "${dir}/*.cpp")
   source_group (${dir} FILES ${s_${dir}} ${h_${dir}})
   set (sources ${sources} ${s_${dir}})
   set (header ${headers} ${h_${dir}})
endforeach ()

add_executable(${NAME} ${sources})

target_link_libraries (${NAME} ${SFML_LIBRARIES})
