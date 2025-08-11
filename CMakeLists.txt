cmake_minimum_required(VERSION 3.10)
project(Quasar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)

include_directories(include)

file(GLOB SOURCES src/*.cc)

add_executable(quasar ${SOURCES})

target_link_libraries(quasar CURL::libcurl)
