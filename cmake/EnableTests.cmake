message(STATUS "Configuring target '${TESTER_NAME}'")

set(MIN_SFML_VERSION 20400)

if(SFML_VERSION LESS MIN_SFML_VERSION)
  MESSAGE(FATAL_ERROR "Testbed requires minimal SFML version '${MIN_SFML_VERSION}', is '${SFML_VERSION}'")
endif()

add_executable(${TESTER_NAME} ${TESTBED_SOURCE_FILES})
target_link_libraries (${TESTER_NAME} ${EXTERNAL_LIBRARIES} ${PROJECT_NAME})
