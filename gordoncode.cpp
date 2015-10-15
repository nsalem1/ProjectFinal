//#ifdef OPENGLFUNC_CPP
//#define OPENGLFUNC_CPP


#include <ctime>
#include <cstring> // memcpy()

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>



#include <unistd.h>
extern "C" {
#include "fonts.h"
}


#include "ppm.h"

#include <iostream> 
#include <cstdlib> // EXIT macro


//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
//unsigned int upause=0;
double timeDiff(struct timespec *start, struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec ) +
			(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------



//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

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


void initXWindows(void) 
{
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


/// IMAGE TEXTURE STUFF

unsigned char *buildAlphaData(Ppmimage *img)
{
	//add 4th component to RGB stream...
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	//newdata = (unsigned char *)malloc(img->width * img->height * 4);
	newdata = new unsigned char[img->width * img->height * 4];
	ptr = newdata;
	for (int i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		//
		//get the alpha value
		//
		//original code
		//get largest color component...
		//*(ptr+3) = (unsigned char)((
		//		(int)*(ptr+0) +
		//		(int)*(ptr+1) +
		//		(int)*(ptr+2)) / 3);
		//d = a;
		//if (b >= a && b >= c) d = b;
		//if (c >= a && c >= b) d = c;
		//*(ptr+3) = d;
		//
		//new code, suggested by Chris Smith, Fall 2013
		*(ptr+3) = (a|b|c);
		//
		ptr += 4;
		data += 3;
	}
	return newdata;
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


	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	
	//Set the screen background color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();

}



//#endif