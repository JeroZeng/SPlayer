all : playDemo testQueue

playDemo : playDemo.cpp ../SPlayer.cpp ../base/Demuxer.cpp ../base/Decoder.cpp ../base/Render.cpp ../base/Queue.cpp ../demuxers/Mp4Demuxer.cpp ../demuxers/RawYUV.cpp
	clang++ playDemo.cpp ../SPlayer.cpp ../base/Demuxer.cpp ../base/Decoder.cpp ../base/Render.cpp ../base/Queue.cpp ../demuxers/Mp4Demuxer.cpp ../demuxers/RawYUV.cpp -o ../playDemo

testQueue : testQueue.cpp ../base/Queue.cpp
	clang++ testQueue.cpp ../base/Queue.cpp -o ../testQueue
