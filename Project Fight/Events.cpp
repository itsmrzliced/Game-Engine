#include "Events.h"


bool Events::initialize(void) {

	memset(m_prev_input, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
	memcpy(m_current_input, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);

	return true;

}

void Events::update(Engine* engine) {

	SDL_PollEvent(&e);

	memcpy(m_prev_input, m_current_input, sizeof(Uint8) * SDL_NUM_SCANCODES);
	memcpy(m_current_input, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);

	if (e.type == SDL_QUIT) {

		engine->close();

	}

	if (e.type == SDL_MOUSEBUTTONDOWN) {



	}

	if (e.type == SDL_MOUSEMOTION) {



	}

}

bool Events::is_key_triggered(const SDL_Scancode keys) const {

	return (m_current_input[keys] == 1 && m_prev_input[keys] == 0);

}

bool Events::is_key_pressed(const SDL_Scancode keys) const {

	return (m_current_input[keys] == 1);

}

bool Events::is_key_released(const SDL_Scancode keys) const {

	return (m_current_input[keys] == 0 && m_prev_input[keys] == 1);

}