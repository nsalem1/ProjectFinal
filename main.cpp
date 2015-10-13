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


// Gordon's timer & x11/opengl code
#include "gordoncode.cpp"


int check_keys(XEvent *e, Game * game);

// move/collision/accel calls
void physics(Game * game);
void render(Game * game);


using std::cout;
using std::endl;



#define GRAVITY 1
#define MAX_VELOCITY 10
#define INITIAL_VELOCITY 1

int main()
{
	initXWindows();
	init_opengl();
	
	cout << "start game" << endl;
	
	Game game;
	
	game.setPos(window_width/2, window_height/2);
	
	game.setResolution(window_width, window_height);
	
	
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	
	
	while(game.run)
	{
		// check input
		while(XPending(dpy))
		{
			XEvent e;
			XNextEvent(dpy, &e);
			check_keys(&e, &game);
			check_resize(&e);
		}
		
		// asteroids timer
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		
		
		while(physicsCountdown >= physicsRate) {
			physics(&game);
			physicsCountdown -= physicsRate;
		}

		render(&game);
		glXSwapBuffers(dpy, win);
	}	
	
	cleanupXWindows();
	//cleanup_fonts();
	
	cout << "end game" << endl;
	return 0;
}



int check_keys(XEvent *e, Game * game)
{
	int key = XLookupKeysym(&e->xkey, 0);
	
	cout << "" << endl;
	// 0 - a left
	// 1 - w up
	// 2 - d right
	// 3 - s down
	// 4 - spacebar
	
	/*if(e->type == KeyRelease) 
	{
		if(key == XK_Up)
		{
			keys[1] = 0;
		}
		if(key == XK_Down)
			keys[3] = 0;
		if(key == XK_Left)
		{
			keys[0] = 0;
		}
		if(key == XK_Right)
			keys[2] = 0;
		
		return 0;
	}*/
	
	if(e->type == KeyPress)
	{
		if(key == XK_Up)
			game->accelX(-1 * INITIAL_VELOCITY);
		if(key == XK_Down)
			game->accelY(INITIAL_VELOCITY);
		if(key == XK_Left)
			game->accelX(INITIAL_VELOCITY);
		if(key == XK_Right)
			game->accelY(-1 * INITIAL_VELOCITY);
		if(key == XK_Escape)
		{
			game->run = false;
		}
	}
	else
		game->setAccel(0,0);
	
	
	return 0;
}

void physics(Game * game)
{
	// 0 - a left
	// 1 - w up
	// 2 - d right
	// 3 - s down
	// 4 - spacebar
	
	
	if(keys[0])
	{
		
	}
	if(keys[1])
	{
		
	}
	if(keys[2])
	{
		
	}
	if(keys[3])
	{
		
	}
	
	game->checkscreenedge();
	
	game->move();
}

void render(Game * game)
{
	float w, h;
	glClear(GL_COLOR_BUFFER_BIT);
	
	//draw guy/rectangle
	glColor3ub(90,140,90);

	glPushMatrix();
	glTranslatef(game->player.position.x, game->player.position.y, 0);
	w = game->player.width;
	h = game->player.height;
	glBegin(GL_QUADS);
	glVertex2i(-w,-h);
	glVertex2i(-w, h);
	glVertex2i( w, h);
	glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
}
