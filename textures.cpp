// cs335 Fall 2015
// ==========================
// student: Pedro Gonzalez
// ==========================

// Used lab2_rainforest texture code as template
// then used online sources for further explaination of code/magic

// sites I used to learn some of these commands/args
// https://open.gl/textures
// OLD!!! from 2007, not exactly correct but helpful
// http://www.gamedev.net/page/resources/_/technical/opengl/rendering-efficient-2d-sprites-in-opengl-using-r2429

// using this site https://convertio.co/png-ppm/ for png to ppm conversion 
// Skeleton spritesheet by MoikMellah
// http://opengameart.org/content/mv-platformer-skeleton
// platformer spritesheet by clint bellanger
// http://opengameart.org/content/platformer-animations



Ppmimage * skeletonBase = NULL;
GLuint skeletonTexture;

Ppmimage *background = NULL;
GLuint backgroundTexture;

GLuint silhouetteTexture;

void loadTextures()
{
	// load image from ppm structure
	skeletonBase = ppm6GetImage("./images/skeletonBase.ppm");
	background = ppm6GetImage("./images/background.ppm");
	
	
	// generate opengl texture element
	glGenTextures(1, &skeletonTexture);
	glGenTextures(1, &backgroundTexture);
	glGenTextures(1, &silhouetteTexture);
	
	
	/////////////////// Skeleton sprite texture ////////////////////////
	
	glBindTexture(GL_TEXTURE_2D, skeletonTexture);
	// bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// write to video memory
	glTexImage2D(GL_TEXTURE_2D, 0, 3, skeletonBase->width, skeletonBase->height,
				0, GL_RGB, GL_UNSIGNED_BYTE, skeletonBase->data);

	
	//////////////////// silhouette///////////////////////
	
	glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	unsigned char *silhouetteData = buildAlphaData(skeletonBase);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, skeletonBase->width, skeletonBase->height, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	delete [] silhouetteData;
	
	
	////////////// GAME BACKGROUND ////////////////////////////
	
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
					background->width, background->height,
					0, GL_RGB, GL_UNSIGNED_BYTE, background->data);
	
	
}


float frame = 1;


// framerate determines how the sprite is drawn
void drawSkeleton(Game * game)
{
	// glTexCoord2i int
	// glTexCoord2f float
	
	

	glPushMatrix();
	glTranslatef(game->player.position.x, game->player.position.y + game->player.width*5, 0);
	glBindTexture(GL_TEXTURE_2D, skeletonTexture);

	
	glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	
	int wid = 10 * game->player.width;//skeletonBase->width;
	//int h = skeletonBase->height;
	
	cout << frame << endl;
		
	if(frame > .7)
		frame = 0.0;
	
	if(game->player.velocity.x == 0)
		frame = 0; // starting position

	if(game->player.left)
		glScalef( -1.0f, 1, 1 );
	
	if(game->player.right)
		glScalef( 1.0f, 1, 1 );
	
	if(game->player.velocity.y > 0) // up
		frame = 0.8;
	
	if(game->player.velocity.y < 0)
		frame = 0.7;
	
	glBegin(GL_QUADS);
	
	
	/*glTexCoord2f(0.0f, 0.1); glVertex2i(-wid,-wid);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f( 0.1f, 0.0f); glVertex2i( wid, wid);
				glTexCoord2f(0.1f, 0.1f); glVertex2i( wid,-wid);*/

				
	glTexCoord2f(frame, 0.1); glVertex2i(-wid,-wid);
	glTexCoord2f(frame, 0); glVertex2i(-wid, wid);
	glTexCoord2f(frame + 0.1, 0); glVertex2i( wid, wid);
	glTexCoord2f(0.1 + frame, 0.1f); glVertex2i( wid,-wid);

	glEnd();
	glPopMatrix();
	
	glDisable(GL_ALPHA_TEST);
	
}

