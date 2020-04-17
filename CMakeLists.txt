cmake_minimum_required(VERSION 3.10)
project(Desktop)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include/cpprestsdk)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src/DesktopCore)
add_subdirectory(src/DesktopApp)