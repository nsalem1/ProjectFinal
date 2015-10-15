# cs335 Final Project
# to compile your project, type make and press enter

all: main

main: main.cpp
	g++ main.cpp game.cpp ppm.cpp libggfonts.a -Wall -omain -lX11 -lGL -lGLU -lm

clean:
	rm -f main
	rm -f *.o
