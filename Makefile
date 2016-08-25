MacOS = Darwin
Linux = Linux

ARCH = $(shell uname -s)

INCLUDES = -Iex_lib -Iex_lib/GLFW

BASE_OBJS = Base.o Queue.o Demuxer.o Decoder.o Render.o
DEMUXERS_OBJS = RawYUV.o H264Demuxer.o Mp4Demuxer.o Mpegts.o
DECODERS_OBJS = H264Decoder.o
RENDERS_OBJS = PCRender.o glad.o

SRC = test/playDemo.cpp							\
     *.cpp										\
     base/*.cpp									\
     demuxers/*.cpp								\
     decoders/*.cpp    	                        \
     renders/*.cpp

ifeq ($(ARCH), $(MacOS))
CXXFLAGS= -arch i386 -m32
LDFLAGS = -framework Cocoa 						\
          -framework IOKit						\
          -framework CoreVideo					\
          -framework OpenGL						\
          -framework Carbon						\
          -Lex_lib/GLFW/lib                     \
          -lglfw3                               \
          -Lex_lib/openh264/                    \
          -lcommon                              \
          -ldecoder                             \
          -read_only_relocs suppress
CXX = clang++
else ifeq ($(ARCH), $(Linux))
CXX = g++
endif

all : playDemo
.PHONY: all
testQueue : test/testQueue.cpp base/Queue.cpp
	clang++ test/testQueue.cpp base/Queue.cpp -o testQueue

playDemo: libsplayer test/playDemo.cpp
	$(CXX) -c $(CXXFLAGS) test/playDemo.cpp -o playDemo.o $(INCLUDES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) playDemo.o -L. -lsplayer -o playDemo
	rm *.o

# make libsplayer
libsplayer: SPlayer.cpp $(BASE_OBJS) $(DEMUXERS_OBJS) $(DECODERS_OBJS)\
			$(RENDERS_OBJS)
	$(CXX) -c $(CXXFLAGS) SPlayer.cpp -o SPlayer.o $(INCLUDES)
	$(AR) rcs libsplayer.a SPlayer.o $(BASE_OBJS) $(DEMUXERS_OBJS)	$(DECODERS_OBJS) $(RENDERS_OBJS)
	rm *.o

%.o: base/%.cpp
	#@echo "Compiling $<..."
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INCLUDES)

%.o: demuxers/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INCLUDES)

%.o: decoders/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INCLUDES)

%.o: renders/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INCLUDES)

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf playDemo
	rm -rf testQueue
