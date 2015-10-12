// CS335 fall 2015
// ====================
// Final Project - Group 2
// Game: Doodle Jump
// Student: Pedro Gonzalez
// 
// ---------------------
// used the asteroids game as a template
// for animation/physics

// game class
#include "game.h"

// GLOBAL vars
int window_width = 800;
int window_height = 600;

// 5 input keys
int keys[5];
// 0 - a left
// 1 - w up
// 2 - d left
// 3 - s down
// 4 - spacebar


// Gordon's x11/opengl code
#include "gordoncode.cpp"


int check_keys(XEvent *e);

// move/collision/accel calls
void physics(Game * game);
void render(Game * game);


using std::cout;
using std::endl;

int main()
{
	initXWindows();
	init_opengl();
	
	cout << "start game" << endl;
	
	Game game;
	game.setResolution(window_width, window_height);
	
	while(game.run)
	{
		// check input
		while(XPending(dpy))
		{
			XEvent e;
			XNextEvent(dpy, &e);
			check_keys(&e);
			check_resize(&e);
		}
		
		// check time/physics
		//while(
		// draw
		
	}	
	
	cout << "end game" << endl;
	return 0;
}

int check_keys(XEvent *e)
{
		
}
