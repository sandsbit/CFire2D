cmake_minimum_required(VERSION 3.31)
project(CFire2D)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.87 COMPONENTS unit_test_framework log exception REQUIRED)
find_package(OpenAL REQUIRED)
include_directories(${Boost_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR} ./CFire2D/)

set(CMAKE_WARN_DEPRECATED FALSE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0")

add_library(CFire2DAudio SHARED CFire2D/audio/openal/openal.cpp CFire2D/audio/openal/openal.h
        CFire2D/audio/audio_abstracts.h
        CFire2D/audio/audio_objects.cpp
        CFire2D/audio/audio_objects.h
        CFire2D/audio/exceptions/unsupported_audio_format.cpp
        CFire2D/audio/exceptions/unsupported_audio_format.h
        CFire2D/audio/exceptions/openal_error.cpp
        CFire2D/audio/exceptions/openal_error.h
        CFire2D/audio/loaders/wave.cpp
        CFire2D/audio/loaders/wave.h
        CFire2D/audio/loaders/audiofile.h
        CFire2D/audio/exceptions/invalid_audio_file_format.cpp
        CFire2D/audio/exceptions/invalid_audio_file_format.h)
target_link_libraries(CFire2DAudio LINK_PUBLIC ${Boost_LIBRARIES} ${OPENAL_LIBRARY})

add_executable(CFire2DTests test/testmain.cpp
        test/audio/loaders/wavetests.h
)
target_link_libraries(CFire2DTests LINK_PUBLIC ${Boost_LIBRARIES} ${OPENAL_LIBRARY} CFire2DAudio)
target_include_directories(CFire2DTests PRIVATE ./test/)

add_executable(CFireSampleSound samples/audio/sound_main.cpp)
target_link_libraries(CFireSampleSound LINK_PUBLIC ${Boost_LIBRARIES} ${OPENAL_LIBRARY} CFire2DAudio)

add_executable(CFireSampleMusic samples/audio/music_main.cpp)
target_link_libraries(CFireSampleMusic LINK_PUBLIC ${Boost_LIBRARIES} ${OPENAL_LIBRARY} CFire2DAudio)