cmake_minimum_required(VERSION 2.8)

project(SPlayer)

add_subdirectory(base)
add_subdirectory(demuxers)
add_subdirectory(decoders)
add_subdirectory(renders)

include_directories(${PROJECT_SOURCE_DIR}/ex_lib)
include_directories(${PROJECT_SOURCE_DIR}/ex_lib/GLFW)

add_executable(playDemo test/playDemo.cpp
                        SPlayer.cpp)
