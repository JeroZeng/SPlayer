cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/ex_lib)
include_directories(${PROJECT_SOURCE_DIR}/ex_lib/GLFW)

add_library(renders
            PCRender.cpp PCRender.h
			glad.cpp)