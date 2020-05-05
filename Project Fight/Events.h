#pragma once

#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <string>

#include <SDL/SDL.h>

#include "Engine.h"

class Events
{

public:

	void update(Engine* engine);

	bool initialize(void);

	bool is_key_triggered(const SDL_Scancode keys) const;
	bool is_key_pressed(const SDL_Scancode keys) const;
	bool is_key_released(const SDL_Scancode keys) const;

private:

	SDL_Event e;

	Uint8 m_prev_input[SDL_NUM_SCANCODES];
	Uint8 m_current_input[SDL_NUM_SCANCODES];

};

#endif