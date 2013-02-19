CC=g++
CFLAGS=-g -Wall -O3 -fast
IFLAGS=-I/usr/X11R6/include
LFLAGS=-L/usr/X11R6/lib -lX11
all: wireframe

wireframe: wireframe.cc graphics.cc algebra.cc sort.cc draw.cc mesh.cc generate.cc input_output.cc interface.cc
	$(CC) $(CFLAGS) $(IFLAGS) -o wireframe wireframe.cc $(LFLAGS) -lm

clean: rm wireframe
