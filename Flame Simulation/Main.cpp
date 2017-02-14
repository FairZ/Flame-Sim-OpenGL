#include <SDL.h>
#include <iostream>
#include <string>
#include "glew.h"

#undef main

bool GL_Initialization();

int main(void)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL Failed to Initialize" << "\n";
		return -1;
	}

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	int winPosX = 320;
	int winPosY = 180;
	int winWidth = 1280;
	int winHeight = 720;

	SDL_Window* window = SDL_CreateWindow("Flame Simulation", winPosX, winPosY, winWidth, winHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, 0 );
	SDL_GLContext glcontext = SDL_GL_CreateContext( window );

	if (!GL_Initialization())
	{
		return -2;
	}

	int previousTime = SDL_GetTicks();
	float deltaTime = 0;

	bool play = true;

	glClearColor(0.0f,0.0f,0.0f,0.0f);

	while (play)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				play = false;
				break;
			default:
				break;
			}
		}


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow( window );

		deltaTime = SDL_GetTicks() - previousTime;
		previousTime = SDL_GetTicks();
		if (deltaTime < 16)
		{
			SDL_Delay(16-deltaTime);
		}
	}

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

bool GL_Initialization()
{

	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cout << "GLEW Initialization Error: " << glewGetErrorString(error) << "\n";
		return false;
	}

	std::cout<<"GLEW Version: "<< glewGetString(GLEW_VERSION)<< "\n";
	std::cout<<"OpenGL Vendor: "<< glGetString( GL_VENDOR ) << "\n";
	std::cout<<"OpenGL Renderer: "<< glGetString( GL_RENDERER ) << "\n";
	std::cout<<"OpenGL Version: "<< glGetString( GL_VERSION ) << "\n";
	std::cout<<"OpenGL Shading Language Version: "<< glGetString( GL_SHADING_LANGUAGE_VERSION ) << "\n";

	glEnable(GL_DEPTH_TEST);

}