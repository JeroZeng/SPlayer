MacOS = Darwin
Linux = Linux

ARCH = $(shell uname -s)

SRC = playDemo.cpp								\
     ../*.cpp									\
     ../base/*.cpp								\
     ../demuxers/*.cpp							\
     ../decoders/*.cpp                          \
     ../renders/*.cpp

ifeq ($(ARCH), $(MacOS))
CXXFLAGS= -framework Cocoa 						\
          -framework IOKit						\
          -framework CoreVideo					\
          -framework OpenGL						\
          -framework Carbon						\
          -I../ex_lib							\
          -L../ex_lib/GLFW/lib					\
          -lglfw3								\
          -L../ex_lib/openh264/                 \
          -lcommon                              \
          -ldecoder                             \
          -I.                                   \
          -m32                                  \
          -arch i386                            \
          -read_only_relocs suppress
CXX = clang++
else ifeq ($(ARCH), $(Linux))
CXX = g++
endif

all : playDemo testQueue testGLSL

playDemo : $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o ../playDemo

testQueue : testQueue.cpp ../base/Queue.cpp
	clang++ testQueue.cpp ../base/Queue.cpp -o ../testQueue

testGLSL : testGLSL.cpp glad.c
	$(CXX) $(CXXFLAGS) testGLSL.cpp glad.c -o testGLSL
