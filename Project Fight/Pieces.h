#pragma once

#ifndef PIECES_H
#define PIECES_H

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <cmath>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Engine.h"
class Engine;

#include "Events.h"
class Events;
#include "Board.h"
class Board;
class Pieces
{

public:

	Pieces(std::string name, std::string color, std::tuple<int, int> pos, Engine* engine, int score);
	~Pieces();

	std::tuple<int, int> get_coords(std::tuple<int, int> pos);
	std::tuple<int, int> get_pos(std::tuple<int, int> coords);
	std::tuple<int, int> return_pos();
	SDL_Texture* texture;
	SDL_Rect srcrect, dstrect;

	std::string get_name();
	std::string get_color();

	bool selected = false;

	bool first_move = true;

	std::tuple<int, int> pos;
	std::string color;
	std::string name;


	std::tuple<int, int> coords;

	int w = 64; int h = 64;
	int score;

	bool passant = false;
	int passant_turn = 1;

	bool white_king_first_move = true;
	bool white_rook_a_first_move = true;
	bool white_rook_h_first_move = true;
	bool black_king_first_move = true;
	bool black_rook_a_first_move = true;
	bool black_rook_h_first_move = true;
	bool in_check = false;


private:

	

	

};

#endif 