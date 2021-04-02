#pragma once

#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include <SDL/SDL.h>

#include "Engine.h"
class Engine;
#include "Pieces.h"
class Pieces;
#include "Board.h"
class Board;
class Events
{

public:

	void update(Engine* engine, Board* board, Pieces* pieces);

	bool initialize(void);

	bool is_key_triggered(const SDL_Scancode keys) const;
	bool is_key_pressed(const SDL_Scancode keys) const;
	bool is_key_released(const SDL_Scancode keys) const;

	bool is_button_triggered(const Uint32 uButton) const;
	bool is_button_pressed(const Uint32 uButton) const;
	bool is_button_released(const Uint32 uButton) const;

	std::tuple<int, int> get_mouse();

private:

	SDL_Event e;

	Uint8 m_prev_input[SDL_NUM_SCANCODES];
	Uint8 m_current_input[SDL_NUM_SCANCODES];

	int m_iCurrentCoordX;
	int m_iCurrentCoordY;
	Uint32 m_uCurrentMouseState;
	int m_iPreviousCoordX;
	int m_iPreviousCoordY;
	Uint32 m_uPreviousMouseState;

	std::tuple<int, int> mouse;

};

#endif