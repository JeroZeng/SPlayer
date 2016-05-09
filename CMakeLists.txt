cmake_minimum_required(VERSION 2.8)

project(SPlayer)

add_subdirectory(base)
add_subdirectory(demuxers)
add_subdirectory(decoders)
add_subdirectory(renders)

add_executable(playDemo test/playDemo.cpp
                        SPlayer.cpp)
