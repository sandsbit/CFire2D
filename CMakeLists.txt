cmake_minimum_required(VERSION 3.31)
project(CFire2D)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.87 COMPONENTS unit_test_framework log REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_library(CFire2D SHARED CFire2D/library.cpp)

target_link_libraries(CFire2D LINK_PUBLIC ${Boost_LIBRARIES})