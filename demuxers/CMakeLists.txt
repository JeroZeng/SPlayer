cmake_minimum_required(VERSION 2.8)

add_library(demuxers
            RawYUV.cpp RawYUV.h
			H264Demuxer.cpp H264Demuxer.h
			Mp4Demuxer.cpp Mp4Demuxer.h)