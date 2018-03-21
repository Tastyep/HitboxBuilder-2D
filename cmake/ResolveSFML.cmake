message(STATUS "[SFML] Resolving internal dependency")

set(MIN_SFML_VERSION 2.3.2)

find_package(SFML ${MIN_SFML_VERSION} REQUIRED system window graphics audio)

list(APPEND EXTERNAL_LIBRARIES ${SFML_LIBRARIES})

message(STATUS "[SFML] Using ${SFML_VERSION}")
