#pragma once

#ifndef BOARD_H
#define BOARD_H

#include "Engine.h"
#include "Render.h"
#include "Pieces.h"
class Pieces;

class Engine;
class Render;

#include <iostream>
#include <vector>
#include <tuple>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

class Board
{

public:

	Board(Engine* engine, Render* render);
	~Board();

	void draw(Engine* engine);

	std::vector<std::tuple<int, int>> return_grid(), return_occupied();

	int turn = 1;

private:

	SDL_Texture* texture;
	SDL_Rect dstrect;
	std::vector<std::tuple<int, int>> grid, occupied_pos;

};

#endif