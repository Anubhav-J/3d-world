CC = g++
CFLAGS = -Wall
PROG = ./game

SRCS = game.cpp imageloader.cpp
LIBS = -lglut -lGL -lGLU -w

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
