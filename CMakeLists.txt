cmake_minimum_required(VERSION 3.31)
project(CFire2D)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.87 COMPONENTS unit_test_framework log exception REQUIRED)
include_directories(${Boost_INCLUDE_DIR} ./CFire2D/)

find_package(OpenAL REQUIRED)

add_library(CFire2DAudio SHARED CFire2D/audio/openal/openal.cpp CFire2D/audio/openal/openal.h
        CFire2D/audio/audio_abstracts.h
        CFire2D/audio/audio_objects.cpp
        CFire2D/audio/audio_objects.h
        CFire2D/audio/exceptions/unsupported_audio_format.cpp
        CFire2D/audio/exceptions/unsupported_audio_format.h)
target_link_libraries(CFire2DAudio LINK_PUBLIC ${Boost_LIBRARIES} ${OPENAL_LIBRARY})