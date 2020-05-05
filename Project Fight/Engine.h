#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h.>
#include <SDL/SDL_mixer.h>


class Engine
{

public:

	Engine(int width, int height, std::string title);
	~Engine();

	SDL_Window* get_window();
	SDL_Renderer* get_renderer();

	int get_width();
	int get_height();
	bool get_running();
	bool collision(SDL_Rect rect1, SDL_Rect rect2);
	bool collision_range(SDL_Rect rect1, SDL_Rect rect2, int x, int y);

	void close();

private:

	SDL_Window* window;
	SDL_Renderer* renderer;

	int width, height; 

	bool running = true; 

};

#endif