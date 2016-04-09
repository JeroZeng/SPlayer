SRC = playDemo.cpp								\
     ../*.cpp									\
     ../base/*.cpp								\
     ../demuxers/*.cpp							\
     ../renders/*.cpp

CXXFLAGS= -framework Cocoa 						\
          -framework IOKit						\
          -framework CoreVideo					\
          -framework OpenGL						\
          -framework Carbon						\
          -I../ex_lib/							\
          -L../ex_lib/GLFW/lib					\
          -lglfw3

CXX=clang++

all : playDemo testQueue

playDemo : $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o ../playDemo

testQueue : testQueue.cpp ../base/Queue.cpp
	clang++ testQueue.cpp ../base/Queue.cpp -o ../testQueue
