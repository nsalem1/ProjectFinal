# cs335 hw1
# to compile your project, type make and press enter

all: game

game: game.cpp
	g++ game.cpp libggfonts.a -Wall -ogame -lX11 -lGL -lGLU -lm

clean:
	rm -f game
	rm -f *.o

