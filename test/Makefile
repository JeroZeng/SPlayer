SRC = playDemo.cpp ../*.cpp ../base/*.cpp ../demuxers/*.cpp ../renders/*.cpp

CXXFLAGS= -framework GLUT \
          -framework OpenGL

CXX=clang++

all : playDemo testQueue

playDemo : $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o ../playDemo

testQueue : testQueue.cpp ../base/Queue.cpp
	clang++ testQueue.cpp ../base/Queue.cpp -o ../testQueue
