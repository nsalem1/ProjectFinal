// cs350 Fall 2015 
// ======================
// Game Project Final
// 
// student: Pedro Gonzalez
// 
// 

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
extern "C" {
#include "fonts.h"
}


//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

//Structures

struct Vec {
	float x, y, z;
};

struct Shape {
	float width, height;
	float radius;
	Vec center;
	
};

struct Box {
	float width, height;
	float radius;
	Vec velocity;
	Vec center;
};

struct Particle {
	Shape s;
	Vec velocity;
};

struct Guy
{
	Shape s; 
};

struct Game {
	bool space;
	Box box;
	Shape circle;
	Particle particle;
	int n;
	int lastMousex;
	int lastMousey;
};

//Function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_mouse(XEvent *e, Game *game);
int check_keys(XEvent *e, Game *game);
void movement(Game *game);
void render(Game *game);
void check_resize(XEvent *e);


// MY FUNCTIONS, gives the box velocity, doesn't actually move until called in movement();
void moveBox(Game *game, char const * direction);



int window_width = 1000;
int window_height = 600;
#define GUY_SIZE 10 // not used yet

// affects the y axis
#define GRAVITY 0.1


#define BOX_WIDTH 5
#define BOX_HEIGHT 20

#define MAX_VELOCITY 3 // how fast the rectangle moves
#define INTITIAL_VELOCITY 2  // how fast movement starts again

int main(void)
{
	int done=0;
	srand(time(NULL));
	initXWindows();
	init_opengl();


	//declare game object
	Game game;
	game.n=0;

	// used to check if jumped, already in the air
	game.space = false; // deny jumping til collision on floor
	
	
	//declare a box shape
	game.box.width = BOX_WIDTH;
	game.box.height = BOX_HEIGHT;
	game.box.center.x = 20;//120 + 5*65;
	game.box.center.y = 10;//500 - 5*60;
	game.box.velocity.y = 0;
	game.box.velocity.x = 0;
	//start animation
	while(!done) {
		while(XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			check_mouse(&e, &game);
			check_resize(&e);
			done = check_keys(&e, &game);
		}
		
		movement(&game);
		render(&game);
		glXSwapBuffers(dpy, win);
	}
	
	
	cleanupXWindows();
	return 0;
}

void set_title(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Game");
}

void cleanupXWindows(void) {
	//do not change
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void initXWindows(void) {
	//do not change
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	int w=window_width, h=window_height;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		std::cout << "\n\tcannot connect to X server\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if(vi == NULL) {
		std::cout << "\n\tno appropriate visual found\n" << std::endl;
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		ButtonPress | ButtonReleaseMask |
		PointerMotionMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
		InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, window_width, window_height);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//Set 2D mode (no perspective)
	glOrtho(0, window_width, 0, window_height, -1, 1);
	//Set the screen background color
	glClearColor(0.1, 0.1, 0.1, 1.0);
}



void setup_screen_res(const int w, const int h)
{
	window_width = w;
	window_height = h;
}

void reshape_window(int width, int height)
{
	//window has been resized.
	setup_screen_res(width, height);
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, window_width, 0, window_height, -1, 1);
	set_title();
}

void check_resize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != window_width || xce.height != window_height) {
		//Window size did change.
		reshape_window(xce.width, xce.height);
	}
}



void makeParticle(Game *game, int x, int y) 
{
	
	std::cout << "makeParticle() " << x << " " << y << std::endl;
	//position of particle


	Particle * p = &game->particle;
	p->s.center.x = x;
	p->s.center.y = y;
	p->velocity.y = GRAVITY;
	p->velocity.x = 0;
	

	game->n++;
}

void check_mouse(XEvent *e, Game *game)
{
	static int savex = 0;
	static int savey = 0;
	static int n = 0;

	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button was pressed
			int y = window_height - e->xbutton.y;
			makeParticle(game, e->xbutton.x, y);
			return;
		}
		if (e->xbutton.button==3) {
			//Right button was pressed
			return;
		}
	}
	//Did the mouse move?
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		savex = e->xbutton.x;
		savey = e->xbutton.y;
		if (++n < 10)
			return;
	}
}

int check_keys(XEvent *e, Game *game)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) 
	{
		int key = XLookupKeysym(&e->xkey, 0);
		if (key == XK_Escape) 
		{
			return 1;
		}
		//You may check other keys here.
		
		
		// check game input
		if(key == XK_Up)
			moveBox(game, "up");
		if(key == XK_Down)
			moveBox(game, "down");
		if(key == XK_Left)
			moveBox(game, "left");
		if(key == XK_Right)
			moveBox(game, "right");
		if(key == XK_space && game->space == true)
		{
			moveBox(game, "space");
		}
	}
	else
		// no key pressed so 
		// apply gravity
		moveBox(game, "null"); // no movement, kill motion
	
	return 0;
}

// this funciton adjusts the axis velocity depending on direction
void moveBox(Game *game, char const * direction)
{
	const char * up = "up";
	const char * down = "down";
	const char * left = "left";
	const char * right = "right";
	const char * space = "space";
	const char * nulmov = "null";
//std::cout << "MOVEMENT" << std::endl;
	if(direction == up)
		game->box.velocity.y += INTITIAL_VELOCITY;

	if(direction == down)
		game->box.velocity.y -= INTITIAL_VELOCITY;
	
	if(direction == left)
		game->box.velocity.x -= INTITIAL_VELOCITY;
	
	if(direction == right)
		game->box.velocity.x += INTITIAL_VELOCITY;
	
	if(direction == space && game->space == true)
	{
		//std::cout << "space" << std::endl;
		//game->space = false;
		game->box.velocity.y = INTITIAL_VELOCITY ; 
	}
	if(direction == nulmov)
	{
		//std::cout << "kill movement" << std::endl;
		if(game->box.velocity.x > -1)
			game->box.velocity.x = 0;
		else
			game->box.velocity.x = 0;
		
		//if(game->box.velocity.y > 0)
			//game->box.velocity.y -= GRAVITY;
		
		//game->box.velocity.y = 0.01;
	}
}

// this function makes sure objects are set to their movement velocity
void movement(Game *game)
{
	
	// moves particle
	Particle * p;

	//if (game->n <= 0)
		//return;


	p = &game->particle;
	p->s.center.x += p->velocity.x;
	p->s.center.y += p->velocity.y;

	//check for collision with shapes...
	//Shape *s;


	//check for off-screen
	if (p->s.center.y < 0.0) {
		//std::cout << "off screen" << std::endl;
		game->n = 0;
	}
	
	// deny multiple jumps
	//game->space = false;
	
	// move box
	Box * b = &game->box;
	
	// check for ability to jumped
	if(b->center.y > 0 && game->space == true)
	{
		game->space = false;
	}
		
	// applies gravity on y axis
	if(b->velocity.y != 0 && b->center.y - b->height > 0)
		b->velocity.y -= 0.05 * GRAVITY;
		
	
	// don't move box if box is already at the bottom of the window
	if(b->center.y - b->height <= 0)
	{
		game->space = true; // allow jumping again
		b->center.y = b->height;
		//b->velocity.y = 0;
	}	

	// if box hits the edges of the window, don't move any further
	if(b->center.x > window_width) // right side
	{
		b->center.x = window_width - b->width;
		b->velocity.x = 0;
	} 
	if(b->center.x <= 0) // left side
	{
		b->center.x = b->width;
		b->velocity.x = 0;
	}
	
	std::cout << "Y velocity " << b->velocity.y << std::endl;
	
	
	/// Cap velocity of x axis
	if(b->velocity.x > MAX_VELOCITY)
		b->velocity.x = MAX_VELOCITY;
	if(b->velocity.x < -1 * MAX_VELOCITY)
		b->velocity.x = -1 * MAX_VELOCITY;
	
	
	
	// set new x,y coordinates
	b->center.x += b->velocity.x;
	b->center.y += b->velocity.y;	
	
}

void render(Game *game)
{
	float w, h;
	glClear(GL_COLOR_BUFFER_BIT);
	//Draw shapes...

	//draw box
	Box *s;
	glColor3ub(90,140,90);
	s = &game->box;
	glPushMatrix();
	glTranslatef(s->center.x, s->center.y, s->center.z);
	w = s->width;
	h = s->height;
	glBegin(GL_QUADS);
	glVertex2i(-w,-h);
	glVertex2i(-w, h);
	glVertex2i( w, h);
	glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//draw all particles here
	glPushMatrix();
	glColor3ub(150,160,220);

	
	Vec *c = &game->particle.s.center;
	w = 2;
	h = 2;
	glBegin(GL_QUADS);
	glVertex2i(c->x-w, c->y-h);
	glVertex2i(c->x-w, c->y+h);
	glVertex2i(c->x+w, c->y+h);
	glVertex2i(c->x+w, c->y-h);
	glEnd();
	glPopMatrix();
	
}



