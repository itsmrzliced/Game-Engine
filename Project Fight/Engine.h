#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h.>
#include <SDL/SDL_mixer.h>

#include "Pieces.h"
class Pieces;
#include "Render.h"
class Render;
#include "Events.h"
class Events;

class Engine
{

public:

	Engine(int width, int height, std::string title);
	~Engine();

	struct Piece {

		std::vector<std::tuple<int, int>> pos;
		std::vector<std::string> name, color;

	};
	SDL_Window* get_window();
	SDL_Renderer* get_renderer();

	int get_width();
	int get_height();
	bool get_running();
	bool mouse_click(SDL_Rect rect, std::tuple<int, int> mouse);
	bool collision(SDL_Rect rect1, SDL_Rect rect2);
	bool collision_range(SDL_Rect rect1, SDL_Rect rect2, int x, int y);

	void update(Events* events, Board* board, Engine* engine, Pieces* pieces);
	void move(Events* events, Board* board, Engine* engine, int index, Pieces* pieces);
	void capture(Events* events, Board* board, Engine* engine, int move_index, int capture_index, Pieces* pieces);
	bool check(Engine* engine, std::tuple<int, int> pos, std::string color, int index);
	Piece find_legal_moves(Board* board, Events* events);

	bool pos_in_range(std::tuple<int, int> pos);

	void draw();

	void close();

	std::vector<Pieces*> get_pieces();
	void destroy_piece(int index);

private:

	SDL_Window* window;
	SDL_Renderer* renderer;

	int width, height; 

	bool running = true; 

	std::vector<Pieces*> pieces;

};

#endif