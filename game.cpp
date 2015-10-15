
#include "game.h"

int PLAYER_WIDTH;
int PLAYER_HEIGHT;


Game::Game()
{
	setPos(30,1000);
	setAccel(0,0);
	gravity = 1;

	powerupTimer = 14;

	powerups = NULL;

	accelY(-1);
	if_jump = false;
	if_hit = false;
	run = true;
}

/*
void addPowerup()
{
int x = RANDOM;
iny y = window_height;

int width = window_width * 0.02; 
int height = window_height * 0.02;

if(powerups == NULL)
{
powerups = new Powerup();
powerups->position, 
return;
}

Powerup * p = powerups;
while(p != NULL)
p = p->next;

p = new Powerup();
}*/

bool Game::inAir()
{
	if(player.position.y > 0 && if_jump == true)
	{
		if_jump = false;
		return true;
	}
	return false;
}

void Game::applyGravity()
{
	if(player.position.y - player.height > 0)
		accelY(-0.25 * gravity);
	//accelY(-1 * gravity);


	Powerup * p = powerups;
	while(p != NULL)
	{
		if(p->position.y - p->height > 0)
			p->velocity.y += -0.25 * gravity;
		p = p->next;
	}
}

void Game::setGravity(int g)
{
	gravity = g;
}


float Game::velY()
{
	return player.velocity.y;
}

float Game::velX()
{
	return player.velocity.x;
}

void Game::setResolution(int x, int y)
{
	window_width = x;
	window_height = y;
	player.width = window_height*0.01;
	player.height = window_height*0.05;
}

void Game::setPos(float x = 0, float y = 0)
{
	player.position.x = x;
	player.position.y = y;
}

void Game::setPosX(float x)
{
	player.position.x = x;
}

void Game::setPosY(float y)
{
	player.position.y = y;
}

void Game::setAccel(float x, float y)
{
	player.velocity.x = x;
	player.velocity.y = y;
}

void Game::accelX(float x)
{
	player.velocity.x += x;
}

void Game::accelY(float y)
{
	player.velocity.y += y;
}

void Game::move()
{
	player.position.x += player.velocity.x;
	player.position.y += player.velocity.y;
}



// ----------------------------------------------

// -----------------------------------------
// collision bottom 
void Game::checkBottomScreen()
{
	// bottom of screen, allow double jump
	if(player.position.y - player.height <= 0) // WORKS!!!
	{
		setPosY(player.height);
		setAccel(velX(),0);
		if_jump = true;
	}

}

bool Game::checkLeftScreenHit()
{
	// left side of screen
	if(player.position.x - player.width < 0)
	{
		setPosX(player.width);
		return true;
	}
	return false;
}

bool Game::checkRightScreenHit()
{
	// right side of screen
	if(player.position.x + player.width >= window_width)
	{
		setPosX(window_width - player.width);
		return true;
	}	
	return false;
}