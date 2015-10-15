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


// check key inputs
int keys[65536];
bool killmovement = false;

// Gordon's timer & x11/opengl code
#include "gordoncode.cpp"


// used for fps counter
int frames = 0;
timespec start;

int check_keys(XEvent *e, Game * game);
void check_mouse(XEvent *e, Game *game);
void physics(Game * game);
void render(Game * game);


using std::cout;
using std::endl;


#define GRAVITY 1
#define MAX_VELOCITY 10
#define INITIAL_VELOCITY 5



// random function
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_PARTICLES 10000
Particle par[MAX_PARTICLES];
int numParticles = 0;
bool bubbler = false;
bool setbackground = false;

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
	clock_gettime(CLOCK_REALTIME, &start);
	
	while(game.run)
	{
		// check input
		XEvent e;
		while(XPending(dpy))
		{
			XNextEvent(dpy, &e);
			check_keys(&e, &game);
			check_resize(&e);
			//check_mouse(&e, &game);
			// if window resets, then the game should handle this event
			game.setResolution(window_width, window_height);
		}
		
		// asteroids timer
		// get lastest time
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		// start time is now current time;
		timeCopy(&timeStart, &timeCurrent);
		
		physicsCountdown += timeSpan;
		// check for collisions, move player
		while(physicsCountdown >= physicsRate) {
			physics(&game);
			physicsCountdown -= physicsRate;
		}
		
		if(frames > 100)
		{
			clock_gettime(CLOCK_REALTIME, &start);
			frames = 0;
		}
		frames++;
		
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
		
	
	killmovement = true;
	if(e->type == KeyRelease) 
	{
		keys[key] = 0;
	}
	
	if(e->type == KeyPress)
	{
		keys[key] = 1;
		
		if(key != XK_Left || key != XK_Right)
			killmovement = false;
		if(key == XK_Escape)
		{
			game->run = false;
		}
		
		if(key == XK_b)
		{
			if(bubbler)
				bubbler = false;
			else
				bubbler = true;
		}	
		
		if(key == XK_w)
			if(setbackground)
				setbackground = false;
			else 
				setbackground = true;
		
	}	
	return 0;
}



void makeParticle(int x, int y) {
	if (numParticles >= MAX_PARTICLES)
		return;
	//std::cout << "makeParticle() " << x << " " << y << std::endl;
	//position of particle
	Particle *p = &par[numParticles];

	p->s.center.x = x;
	p->s.center.y = y;
	
	p->velocity.x =  1.0 + rnd() * 0.1;

	p->velocity.y = rnd() * 1.0 - 0.5;

	numParticles++;
}


// check for button clicks?
void check_mouse(XEvent *e, Game *game)
{

	static int savex = 0;
	static int savey = 0;
	//static int n = 0;

	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button was pressed
	
			return;
		}
		if (e->xbutton.button==3) {
			//Right button was pressed
			//std::cout << "right mouse b down" << std::endl;
			return;
		}
	}
	//Did the mouse move?
	if (savex != e->xbutton.x || savey != e->xbutton.y) 
	{


	}
}


// checks if the player is in the air, hits a screen edge, 
// and handles player speed and movement
void physics(Game * game)
{
	
	
	game->inAir(); 
	game->applyGravity();
	game->checkBottomScreen();

	if(keys[XK_Left]) // left
	{
		//cout << "left" << endl;
		game->accelX(-1 * INITIAL_VELOCITY);
	}

	if(keys[XK_Right]) // right
	{
		//cout << "right" << endl;
 		game->accelX(INITIAL_VELOCITY);
	}

	if(keys[XK_space] && game->if_jump) // spacebar
	{
		//cout << "jump" <<endl;
		game->accelY(2 * INITIAL_VELOCITY);
	}
	 
	if(killmovement && game->inAir()) // kill movement on x axis only
		game->player.velocity.x = 0;
	
	
	
	if(game->velX() > MAX_VELOCITY)
		game->player.velocity.x = MAX_VELOCITY;
	if(game->velX() < -1 * MAX_VELOCITY)
		game->player.velocity.x = -1 * MAX_VELOCITY;
		
	

	
	game->move();
	
	
	
	int x_bubbler = 100;
	int y_bubbler = window_height;

	if(bubbler) // if bubbler is toggled only stream water from the top, no mouse involved
	{
		for(int i = 0; i < window_height * 0.15; i++)
		{
			x_bubbler += rnd()*10;
			makeParticle(x_bubbler, y_bubbler);
		}
	}
	
	// particles
	Particle *p = &par[numParticles];
	
	
	
	for(int i = 0; i < numParticles; ++i)
	{
		p = &par[i];
		p->s.center.x += p->velocity.x;
		p->s.center.y += p->velocity.y;
		p->velocity.y -= 0.1; 
	
		if (p->s.center.y < 0.0 || p->s.center.y > window_height) 
		{
			//std::cout << "off screen" << std::endl;
			memcpy(&par[i], &par[numParticles -1], 
				sizeof(Particle));
			numParticles--;
		}
	}
}



void render(Game * game)
{

	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// texture
	if(setbackground)
	
	{
		glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
		glVertex2i(0,0);
	glTexCoord2f(0,0);
		glVertex2i(0, window_height);
	glTexCoord2f(1,0);
		glVertex2i(window_width, window_height);
	glTexCoord2f(1,1);
		glVertex2i(window_width, 0);
	glEnd();
	
	
	// need this else visual error
	glEnable(GL_TEXTURE_2D);
	}
	
	// fps counter
	Rect r;
	r.bot = window_height - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00FFFF00, "fps: %f", frames/timeDiff(&start, &timeCurrent));
	ggprint8b(&r, 16, 0x00FFFF00, "PhysicsRate: %i", static_cast<int>(1/physicsRate));
	ggprint8b(&r, 16, 0x00FFFF00, "water particles: %i", numParticles);
	
	
	
	//draw guy/rectangle
	game->checkRightScreenHit();
	game->checkLeftScreenHit();
	

	float w, h;
	glColor3ub(222,10,90);
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
	glColor3ub(0,0,255);
	glPushMatrix();
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

	
	// draw particles 
	int randColorWater = 0;
	for(int i = 0; i < numParticles; ++i){

		if(randColorWater == 100)
			randColorWater = 0;
		//glColor3ub(150 ,160 ,255);// BLUE water
		glColor3ub(0+randColorWater ,0+randColorWater ,255);// looks best 
		//glColor3ub(150 + randColorWater, 160 + randColorWater,255); // too light colored
		Vec *c = &par[i].s.center;
		w = 2;
		h = 2;
		glBegin(GL_QUADS);
		glVertex2i(c->x-w, c->y-h);
		glVertex2i(c->x-w, c->y+h);
		glVertex2i(c->x+w, c->y+h);
		glVertex2i(c->x+w, c->y-h);
		glEnd();
		glPopMatrix();	 
		randColorWater+= 10; 
	}

}
