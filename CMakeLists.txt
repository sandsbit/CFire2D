cmake_minimum_required(VERSION 3.31)
project(CFire2D)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.87 COMPONENTS unit_test_framework log REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenAL REQUIRED)

add_library(CFire2DAudio SHARED CFire2D/audio/library.cpp)
target_link_libraries(CFire2DAudio LINK_PUBLIC ${Boost_LIBRARIES} ${OPENAL_LIBRARY})