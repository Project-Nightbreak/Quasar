cmake_minimum_required(VERSION 3.10)
project(Quasar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GUMBO REQUIRED gumbo)

# Include directories
include_directories(
    include
    ${GUMBO_INCLUDE_DIRS}
)

file(GLOB SOURCES src/*.cc)

add_executable(quasar ${SOURCES})

# Link libraries
target_link_libraries(quasar
    CURL::libcurl
    ${GUMBO_LIBRARIES}
)
