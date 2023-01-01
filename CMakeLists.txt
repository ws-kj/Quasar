cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project("quasar" LANGUAGES CXX)

find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

#set(SOURCE_EXE quasar.cpp quasar.h)
include_directories(${CURL_INCLUDE_DIR})

include(GNUInstallDirs)

add_library(quasar
    src/quasar.cpp
    include/quasar.h
)

target_link_libraries(quasar ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)

set_target_properties(quasar PROPERTIES PUBLIC_HEADER 
    "include/quasar.h")

install(
    TARGETS quasar
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

