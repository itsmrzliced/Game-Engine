#pragma once

#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <string>

#include <SDL/SDL.h>

#include "Engine.h"

class Render
{

public:

	Render(Engine* engine);
	~Render();

	void draw(Engine* engine);

	SDL_Texture* load_texture(Engine* engine, std::string path);
	
	TTF_Font* load_font(Engine* engine, std::string path, int size);

	SDL_Texture* load_text(Engine* engine, TTF_Font* font, std::string text, SDL_Color color);

	SDL_Rect load_text_dimensions(Engine* engine, SDL_Texture* texture);

private:


};

#endif