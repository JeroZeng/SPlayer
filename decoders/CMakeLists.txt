cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/ex_lib)

add_library(decoders
            H264Decoder.cpp H264Decoder.h)