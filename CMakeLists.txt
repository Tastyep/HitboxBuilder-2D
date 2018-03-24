cmake_minimum_required (VERSION 2.8.1)
cmake_policy(SET CMP0042 NEW)

set(PROJECT_NAME "HitboxBuilder")
project (${PROJECT_NAME} CXX)

# Global variables
set(MODULE_DIR cmake)
set(SOURCE_DIR src)
set(INCLUDE_DIR include)
set(LIB_DIR lib)
set(IMPLEMENTATION_DIR detail)
set(TESTBED_DIR example)
set(TESTER_NAME "${PROJECT_NAME}-tester")

set(CLANG_TIDY_CONFIG_SOURCE "${PROJECT_SOURCE_DIR}/.clang-tidy")

# CMake properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
set(PREDEFINED_TARGETS_FOLDER "CMake")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/${MODULE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE ${BUILD_TYPE})


message(STATUS "Building ${BUILD_TYPE} ${PROJECT_NAME}\n----------------------------")

# Resolve internal dependency
include(ResolveSFML)

# Set compiler flags
include(CompilerFlags)

# Source files.
include(Sources)

include_directories (${PROJECT_SOURCE_DIR}/${INCLUDE_DIR})

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE_FILES})

if(HITBOX_BUILDER_ENABLE_CLANGTIDY AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  include(EnableClangTidy)
  enable_clangtidy(SOURCES "${LIB_SOURCE_FILES}")
endif()

if(HITBOX_BUILDER_BUILD_TESTBED)
  message(STATUS "Configuring target '${TESTER_NAME}'")
  add_executable(${TESTER_NAME} ${TESTBED_SOURCE_FILES})
  target_link_libraries (${TESTER_NAME} ${EXTERNAL_LIBRARIES} ${PROJECT_NAME})
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${LIB_DIR})
install(FILES ${LIB_EXPOSED_HEADER_FILES} DESTINATION "${INCLUDE_DIR}/${PROJECT_NAME}")
