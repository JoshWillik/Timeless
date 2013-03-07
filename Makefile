CC=g++
CFLAGS=-Wall -g
GLFWFLAGS=$(shell pkg-config --cflags libglfw)
GLFWOUTPUTFLAGS=$(shell pkg-config --libs libglfw glew)
LDFLAGS=
SOURCES=playground.cpp
OUTPUT=runtime.bin
OUTPUT32=32runtime.bin
OUTPUT64=64runtime.bin
OUTPUTFLAGS=-lGL 

all:
	$(CC) $(CFLAGS) $(GLFWFLAGS) $(SOURCES) -o $(OUTPUT) $(OUTPUTFLAGS) $(GLFWOUTPUTFLAGS)

32bit:
	$(CC) -m32 $(CFLAGS) $(GLFWFLAGS) $(SOURCES) -o $(OUTPUT32) $(OUTPUTFLAGS) $(GLFWOUTPUTFLAGS)

64bit:
	$(CC) -m64 $(CFLAGS) $(GLFWFLAGS) $(SOURCES) -o $(OUTPUT64) $(OUTPUTFLAGS) $(GLFWOUTPUTFLAGS)

clean:
	rm *.bin
