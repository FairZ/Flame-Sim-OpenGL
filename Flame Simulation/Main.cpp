#include <iostream>
#include <string>
#include "glew.h"
#include "Scene.h"
#include <time.h>

//SDL requirement
#undef main

//openGL intialization function
bool GL_Initialization();

int main(void)
{
	//initialize SDL, if it doesn't run close the program
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL Failed to Initialize" << "\n";
		return -1;
	}
	//set random seed
	srand(time(NULL));

	//expand for openGL and SDL setup
	#pragma region general openGL and SDL setup

	IMG_Init(IMG_INIT_PNG);

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

	//intialize openGL if it doesn't work close the program
	if (!GL_Initialization())
	{
		return -2;
	}

	//set the clear colour
	glClearColor(0.0f,0.0f,0.0f,0.0f);

    #pragma endregion

	Uint32 deltaTime = 0;

	bool play = true;

	//create scene
	Scene Campsite;

	int previousTime = SDL_GetTicks();

	//game loop
	while (play)
	{
		
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				//if exit is clicked, exit the program
			case SDL_QUIT:
				play = false;
				break;
				//rotate camera based on input
			case SDL_KEYDOWN:
				switch( event.key.keysym.sym )
                {
                    case SDLK_LEFT:
                    Campsite.SetRotateleft(true);
                    break;
                    case SDLK_RIGHT:
                    Campsite.SetRotateRight(true);
                    break;
				}
				break;
			case SDL_KEYUP:
				switch( event.key.keysym.sym )
                {
                    case SDLK_LEFT:
                    Campsite.SetRotateleft(false);
                    break;
                    case SDLK_RIGHT:
                    Campsite.SetRotateRight(false);
                    break;
				}
				break;
			default:
				break;
			}
		}

		//update and draw the scene
		Campsite.Update();
		Campsite.Draw();
		//swap the window
		SDL_GL_SwapWindow( window );

		//update deltaTime
		deltaTime = SDL_GetTicks() - previousTime;
		previousTime = SDL_GetTicks();
		Campsite.SetDeltaTime(deltaTime);
		//frame limiter
		if (deltaTime < 1000/60)
		{
			SDL_Delay((1000/60) - deltaTime);
		}
	}

	//cleanup
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}

bool GL_Initialization()
{
	//basic openGL initializatin through glew
	glewExperimental = GL_TRUE;

	//if there is a problem when initializing show error in console and return false
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cout << "GLEW Initialization Error: " << glewGetErrorString(error) << "\n";
		return false;
	}

	//show data in console
	std::cout<<"GLEW Version: "<< glewGetString(GLEW_VERSION)<< "\n";
	std::cout<<"OpenGL Vendor: "<< glGetString( GL_VENDOR ) << "\n";
	std::cout<<"OpenGL Renderer: "<< glGetString( GL_RENDERER ) << "\n";
	std::cout<<"OpenGL Version: "<< glGetString( GL_VERSION ) << "\n";
	std::cout<<"OpenGL Shading Language Version: "<< glGetString( GL_SHADING_LANGUAGE_VERSION ) << "\n";

	//depth test enabled for simple drawing
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}