MacOS = Darwin
Linux = Linux

ARCH = $(shell uname -s)

SRC = playDemo.cpp								\
     ../*.cpp									\
     ../base/*.cpp								\
     ../demuxers/*.cpp							\
     ../renders/*.cpp

ifeq ($(ARCH), $(MacOS))
CXXFLAGS= -framework Cocoa 						\
          -framework IOKit						\
          -framework CoreVideo					\
          -framework OpenGL						\
          -framework Carbon						\
          -I../ex_lib/							\
          -L../ex_lib/GLFW/lib					\
          -lglfw3
CXX = clang++
else ifeq ($(ARCH), $(Linux))
CXX = g++
endif

all : playDemo testQueue

playDemo : $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o ../playDemo

testQueue : testQueue.cpp ../base/Queue.cpp
	clang++ testQueue.cpp ../base/Queue.cpp -o ../testQueue
