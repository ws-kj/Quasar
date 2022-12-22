cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project("quasar" LANGUAGES CXX)

find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

set(SOURCE_EXE quasar.cpp quasar.h)
include_directories(${CURL_INCLUDE_DIR})

add_library(quasar SHARED src/quasar.cpp)

target_include_directories(quasar PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(quasar ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)

install(TARGETS quasar DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES include/quasar.h DESTINATION ${CMAKE_INSTALL_PREFIX}/../include)
