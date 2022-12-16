cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project("quasar" LANGUAGES CXX)

find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

set(SOURCE_EXE test.cpp quasar.cpp quasar.h)
include_directories(${CURL_INCLUDE_DIR})

add_executable(quasar_test ${SOURCE_EXE})
target_link_libraries(quasar_test ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)
