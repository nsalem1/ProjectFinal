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
int keys[6];
// 0 - a left
// 1 - w up
// 2 - d right
// 3 - s down
// 4 - spacebar
// 5 - null movement/kill accel

// Gordon's timer & x11/opengl code
#include "gordoncode.cpp"

//#include "test.cpp"

int check_keys(XEvent *e, Game * game);

// move/collision/accel calls
void physics(Game * game);


void render(Game * game);


using std::cout;
using std::endl;


#define GRAVITY 1
#define MAX_VELOCITY 10
#define INITIAL_VELOCITY 5

int main()
{
	initXWindows();
	init_opengl();
	
	cout << "start game" << endl;
	
	Game game;
	
	game.setGravity(GRAVITY);
	
	// set players position
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
			
			// if window resets, then the game should handle this event
			game.setResolution(window_width, window_height);
		}
		
		// asteroids timer
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		
	
		// check for collisions, move player
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


// TODO!
// -----------------------
// Kind of working! 
// Needs to check if key pressed is later released, Example user presses LEFT then lets go,
// the code then should kill movement on the x axis
int check_keys(XEvent *e, Game * game)
{
	int key = XLookupKeysym(&e->xkey, 0);
	
	//cout << "" << endl;
	// 0 - a left
	// 1 - w up
	// 2 - d right
	// 3 - s down
	// 4 - spacebar
	// 5 - kill movement
	
	keys[5] = 1; // null movement on x axis only
	
	if(e->type == KeyRelease) 
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
		if(key == XK_space)
			keys[4] = 0;
		
		
		//return 0;
	}
	
	if(e->type == KeyPress)
	{
		if(key == XK_Up)
			keys[1] = 1;
		if(key == XK_Down)
			keys[3] = 1;
		if(key == XK_Left)
			keys[0] = 1;
		if(key == XK_Right)
			keys[2] = 1;
		if(key == XK_Escape)
		{
			game->run = false;
		}
		if(key == XK_space)
			keys[4] = 1;
		
		keys[5] = 0;
	}
	
	
	
	
	return 0;
}


// checks if the player is in the air, hits a screen edge, 
// and handles player speed and movement
void physics(Game * game)
{
	

	game->inAir(); 
	game->applyGravity();
	game->checkscreenedge();

	if(keys[0]) // left
	{
		cout << "left" << endl;
		game->accelX(-1 * INITIAL_VELOCITY);
	}

	if(keys[2]) // right
	{
		cout << "right" << endl;
 		game->accelX(INITIAL_VELOCITY);
	}

	if(keys[4] && game->if_jump) // spacebar
	{
		cout << "jump" <<endl;
		game->accelY(2 * INITIAL_VELOCITY);
	}
	
	
	if(keys[5] && game->inAir()) // kill movement on x axis only
		game->player.velocity.x = 0;
	
	
	
	if(game->velX() > MAX_VELOCITY)
		game->player.velocity.x = MAX_VELOCITY;
	if(game->velX() < -1 * MAX_VELOCITY)
		game->player.velocity.x = -1 * MAX_VELOCITY;
		
	
	game->move();

}



void render(Game * game)
{

	glClearColor(0, 0, 0, 1.0);
	//if(!game->if_jump)
	//	glClearColor(0 + game->velY()/20.0, 0 + game->velY()/20.0, 1, 1.0);


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
	
	//draw guy's (x,y) center coordinates
	glPushMatrix();
	glColor3ub(0,0,255);
	
	int x = game->player.position.x;
	int y = game->player.position.y;
	w = 2;
	h = 2;
	glBegin(GL_QUADS);
	glVertex2i(x-w, y-h);
	glVertex2i(x-w, y+h);
	glVertex2i(x+w, y+h);
	glVertex2i(x+w, y-h);
	glEnd();
	glPopMatrix();
}
