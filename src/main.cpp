/*
	Created by Kevin on 05/27/2017
	Technical Exercise for CreatiWity internship
*/
// Compile : clang++ main.cpp Shape.cpp -lGL -lSDL2 -std=c++11 -o Test

// Generic include
#include <string>
#include <iostream>
#include <stdio.h>

// SDL2 include
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_test_images.h>

// Game Include
#include "Shape.h"

std::string windowName = "CreatiWity Technical Exercise - Amazing VideoGame by Kevin";

// SDL Window
SDL_Window *mainWindow;

// Opengl context handle
SDL_GLContext mainContext;
SDL_Surface* bgSur;

// background image storage
GLuint bgTex = 0;
int bgWidth;
int bgHeight;

// Declare functions
bool setOpenGLAttributes();
void printSDL_GL_Attributes();
void checkSDLError(int line);
void runGame();
void cleanup();
void drawBackground();

// Configuration
#define WINDOW_H 512
#define WINDOW_W 512

// Init the SDL and OpenGL Context
bool Init()
{	
	// Initialize SDL subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}

	// Create window
	mainWindow = SDL_CreateWindow(
		windowName.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_OPENGL
	);

	// Detect errors
	if (!mainWindow)
	{
		std::cout << "Failed to create window\n";
		checkSDLError(__LINE__);
		return false;
	}

	// Attach OpenGL Context for the SDL Window
	mainContext = SDL_GL_CreateContext(mainWindow);

	setOpenGLAttributes();

	SDL_GL_SetSwapInterval(1);

	return true;
}

// Configure OpenGL
bool setOpenGLAttributes()
{
	// Set OpenGL version.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	return true;
}

// Start the program
int main(int argc, char *argv[])
{
	if (!Init())
		return -1;
	
	runGame();

	cleanup();

	return 0;
}

// Main loop for the game
void runGame()
{
	bool loop = true;

	// Load background image
	// Move to external function and check if file does exist (should return texture id or 0)...
	FILE * file;
	file = fopen("bg.bmp", "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, file);

	bgWidth = *(int*)&info[18];
	bgHeight = *(int*)&info[22];

	int size = 3 * bgWidth * bgHeight;
	unsigned char * data = new unsigned char[size];
	fread(data, sizeof(unsigned char), size, file);
	fclose(file);
	
	// Generate texture
	glGenTextures(1, &bgTex);
	glBindTexture(GL_TEXTURE_2D, bgTex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Display mode of the texture to clamping
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); //GL_CLAMP_TO_BORDER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// Store the texture in GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bgWidth, bgHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	free(data);

	// Set current texture to null and quit texture mode
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	// Create the shape (circle)
	Shape circle = Shape(0, 0, 0.1f);

	while (loop)
	{
		// Reset screen
		drawBackground();

		// Handle events
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				loop = false;

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					loop = false;
					break;
				case SDLK_SPACE:
					// Toogle shape visibility
					circle.toggle();
					break;
				case SDLK_LEFT:
					circle.move(-.1, 0.0);
					break;
				case SDLK_RIGHT:
					circle.move(.1, 0.0);
					break;
				case SDLK_UP:
					circle.move(0.0, .1);
					break;
				case SDLK_DOWN:
					circle.move(0.0, -.1);
					break;
				default:
					break;
				}
			}
		}

		// Draws shapes using a specific color.
		// To Improve : make color an attribute of Shape and swap color in the draw method.
		glColor3f(0.0, 1.0, 0.0);
		circle.draw();

		// Update frame
		SDL_GL_SwapWindow(mainWindow);
	}
}

// Reset the screen display and draw the background texture.
// This could be improved by adding a shader to draw colored pixels using the texture.
void drawBackground()
{
	// Reset screen
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bgTex);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, WINDOW_W/2, 0, WINDOW_H/2, 0, 1);
	// FLip the image (texture loaded reversed)
	//glScalef(1, -1, 1);
	//glTranslatef(0, -WINDOW_H, 0);
	glMatrixMode(GL_MODELVIEW);
	// Draw quad with actual texture
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(0, 0);
	glTexCoord2i(0, 1); glVertex2i(0, bgHeight);
	glTexCoord2i(1, 1); glVertex2i(bgWidth, bgHeight);
	glTexCoord2i(1, 0); glVertex2i(bgWidth, 0);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Quit the application.
void cleanup()
{
	// Delete OpengL context
	SDL_GL_DeleteContext(mainContext);

	// Destroy window
	SDL_DestroyWindow(mainWindow);

	// Shutdown SDL 2
	SDL_Quit();
}

// Check init error of the window.
void checkSDLError(int line = -1)
{
	std::string error = SDL_GetError();

	if (error != "")
	{
		std::cout << "SLD Error : " << error << std::endl;

		if (line != -1)
			std::cout << "\nLine : " << line << std::endl;

		SDL_ClearError();
	}
}

// Debug purpose
void printSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}
