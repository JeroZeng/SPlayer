cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/ex_lib)

add_library(base
            Base.cpp Base.h
			Queue.cpp Queue.h
			Demuxer.cpp Demuxer.h
			Decoder.cpp Decoder.h
			Render.cpp Render.h)