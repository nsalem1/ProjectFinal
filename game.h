// CS335 fall 2015
// ===========================
// Final Project: Group 2
// 
// student: Pedro Gonzalez
// 
// ===========================

// game class 
// -----------------
// hold player and platform coordinates,
// manages movement and velocities of 
// game objects, checks for collisions
// prompts start menu, death/restart screen

#ifndef GAME_H
#define GAME_H

#include <iostream>
#define PLAYER_WIDTH 5
#define PLAYER_HEIGHT 20

using std::cout;
using std::endl;

struct Vec
{
	float x;
	float y;
};

struct Guy
{
	Vec position;
	Vec velocity;
	int width;
	int height;
};


class Game
{

	//private:

	public:	
		Guy player;	
		bool if_hit; // collision then true, 
		bool if_jump; // if true allow jumping
		bool run; // runs main loop
		int window_height;
		int window_width;
		int gravity;
	
		// platform object?	

		Game()
		{
			setPos(30,20);
			setAccel(0,0);
			player.width = PLAYER_WIDTH;
			player.height = PLAYER_HEIGHT;
			accelY(-1);
			if_jump = false;
			if_hit = false;
			run = true;
		}
		
		// if the player is in the air, deny multiple jumps
		void inAir()
		{
			if(player.position.y > 0 && if_jump == true)
				if_jump = false;
		}
		
		void applyGravity()
		{
			if(player.position.y - player.height > 0)
				accelY(-0.25 * gravity);
			
		}
		
		void setGravity(int g)
		{
			gravity = g;
		}
		
		float velY()
		{
			return player.velocity.y;
		}
		
		float velX()
		{
			return player.velocity.x;
		}
		
		void setResolution(int x, int y)
		{
			window_width = x;
			window_height = y;
		}
		
		void setPos(float x = 0, float y = 0)
		{
			player.position.x = x;
			player.position.y = y;
		}
		
		void setPosX(float x)
		{
			player.position.x = x;
		}
		
		void setPosY(float y)
		{
			player.position.y = y;
		}
		
		void setAccel(float x, float y)
		{
			player.velocity.x = x;
			player.velocity.y = y;
		}
		
		void accelX(float x)
		{
			player.velocity.x += x;
		}
		
		void accelY(float y)
		{
			player.velocity.y += y;
		}
		
		void move()
		{
			player.position.x += player.velocity.x;
			player.position.y += player.velocity.y;
		}
		
		// collision bottom and left/right of screen
		void checkscreenedge()
		{
			// bottom of screen, allow double jump
			if(player.position.y - player.height <= 0)
			{
				setPosY(player.height );
				setAccel(velX(),0);
				if_jump = true;
			}
			
			// right side of screen
			if(player.position.x > window_width)
			{
				setPosX(window_width - player.width);
				setAccel(0,velY());
			}	
			
			// left side of screen
			if(player.position.x <= 0)
			{
				setPosX(player.width);
				setAccel(0,velY());
			}
		}
		
		// more collision checking with rect platforms??!?!

};


#endif