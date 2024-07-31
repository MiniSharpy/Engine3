#include "Engine.h"
#include <cassert>
#include <print>
#include <SDL.h>

Engine3::Engine::Engine()
{
	// Initialise SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::print("{}\n", SDL_GetError());
		IsInitialised = false;
		assert(false);
	}

	// Set OpenGL version. This should be done before creating an OpenGL window.
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) != 0)
	{
		std::print("{}\n", SDL_GetError());
		IsInitialised = false;
		assert(false);
	}

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3) != 0)
	{
		std::print("{}\n", SDL_GetError());
		IsInitialised = false;
		assert(false);
	}

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0)
	{
		std::print("{}\n", SDL_GetError());
		IsInitialised = false;
		assert(false);
	}

	// Initialise OpenGL attributes.
	if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) != 0)
	{
		std::print("{}\n", SDL_GetError());
		IsInitialised = false;
		assert(false);
	}
}

Engine3::Engine::~Engine() { SDL_Quit(); }

void Engine3::Engine::Update() {}

Engine3::Engine::operator bool() const { return IsInitialised; }
