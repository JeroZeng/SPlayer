MacOS = Darwin
Linux = Linux

ARCH = $(shell uname -s)

INCLUDES = -Iex_lib -Iex_lib/GLFW

BASE_OBJS = Base.o Queue.o Demuxer.o Decoder.o Render.o
DEMUXERS_OBJS = RawYUV.o H264Demuxer.o Mp4Demuxer.o
DECODERS_OBJS = H264Decoder.o
RENDERS_OBJS = PCRender.o glad.o

SRC = test/playDemo.cpp							\
     *.cpp										\
     base/*.cpp									\
     demuxers/*.cpp								\
     decoders/*.cpp    	                        \
     renders/*.cpp

ifeq ($(ARCH), $(MacOS))
CXXFLAGS= -arch i386							\
          -m32                                  
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

all : playDemo testQueue libbase libdemuxers libdecoders libsplayer

playDemo : $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o playDemo

testQueue : test/testQueue.cpp base/Queue.cpp
	clang++ test/testQueue.cpp base/Queue.cpp -o testQueue

player: playDemo.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) playDemo.o -L. -lbase -ldecoders -ldemuxers -lrenders -lsplayer -o player
playDemo.o: test/playDemo.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INCLUDES)

libsplayer: SPlayer.o
	$(AR) rcs libsplayer.a SPlayer.o libbase.a libdemuxers.a libdecoders.a \
librenders.a ex_lib/GLFW/lib/libglfw3.a ex_lib/openh264/libcommon.a ex_lib\
/openh264/libdecoder.a
SPlayer.o: SPlayer.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INCLUDES)

libbase : $(BASE_OBJS)
	$(AR) rcs libbase.a $(BASE_OBJS)
	rm *.o
%.o: base/%.cpp
	#@echo "Compiling $<..."
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INCLUDES)

libdemuxers: $(DEMUXERS_OBJS)
	$(AR) rcs libdemuxers.a $(DEMUXERS_OBJS)
	rm *.o
%.o: demuxers/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INCLUDES)

libdecoders: $(DECODERS_OBJS)
	$(AR) rcs libdecoders.a $(DECODERS_OBJS)
	rm *.o
%.o: decoders/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INCLUDES)

librenders: $(RENDERS_OBJS)
	$(AR) rcs librenders.a $(RENDERS_OBJS)
	rm *.o
%.o: renders/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INCLUDES)
