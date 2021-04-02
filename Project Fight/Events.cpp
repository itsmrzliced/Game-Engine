#include "Events.h"


bool Events::initialize(void) {

	memset(m_prev_input, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
	memcpy(m_current_input, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);

	m_iPreviousCoordX = 0;
	m_iPreviousCoordY = 0;
	m_uPreviousMouseState = 0;
	m_uCurrentMouseState = SDL_GetMouseState(&m_iCurrentCoordX, &m_iCurrentCoordY);

	return true;

}

void Events::update(Engine* engine, Board* board, Pieces* pieces) {

	SDL_PollEvent(&e);

	memcpy(m_prev_input, m_current_input, sizeof(Uint8) * SDL_NUM_SCANCODES);
	memcpy(m_current_input, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);

	m_iPreviousCoordX = m_iCurrentCoordX;
	m_iPreviousCoordY = m_iCurrentCoordY;
	m_uPreviousMouseState = m_uCurrentMouseState;

	///--- Update the current state of the mouse
	m_uCurrentMouseState = SDL_GetMouseState(&m_iCurrentCoordX, &m_iCurrentCoordY);

	if (e.type == SDL_QUIT) {

		engine->close();

	}

	if (is_button_triggered(SDL_BUTTON_LEFT)) {
		
		int x = e.button.x;
		int y = e.button.y;
		this->mouse = std::make_tuple(x, y);
		int piece = -1, capture = -1;

		std::string color;

		if (board->turn % 2 == 1)
			color = "White";
		else {

			color = "Black";

		}

		

		for (int i = 0; i < engine->get_pieces().size(); i++) {

			if (engine->mouse_click(engine->get_pieces()[i]->dstrect, this->mouse) /*&& color == engine->get_pieces()[i]->get_color() && !engine->get_pieces()[i]->selected*/) {
				
				if (color == engine->get_pieces()[i]->get_color())
					piece = i;
				else
					capture = i;

			}

		}

		if (piece != -1) {

			int selected = -1;

			for (int i = 0; i < engine->get_pieces().size(); i++) {

				if (engine->get_pieces()[i]->selected) {

					selected = i;

				}

			}

			if (engine->mouse_click(engine->get_pieces()[piece]->dstrect, this->mouse) && color == engine->get_pieces()[piece]->get_color()){

				engine->get_pieces()[piece]->selected = true;
				if (selected != -1)
					engine->get_pieces()[selected]->selected = false;
			}

			else {

				engine->get_pieces()[piece]->selected = false;

			}

			
		}

		engine->update(this, board, engine, pieces);

		for (int i = 0; i < engine->find_legal_moves(board, this).size(); i++) {
			std::cout << std::get<0>(engine->find_legal_moves(board, this)[i]) << ", " << std::get<1>(engine->find_legal_moves(board, this)[i]) << std::endl;
		}
		std::cout << engine->find_legal_moves(board, this).size() << std::endl;

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

bool Events::is_button_triggered(const Uint32 uButton) const
{
	return ((SDL_BUTTON(uButton) & m_uCurrentMouseState) != 0) && ((SDL_BUTTON(uButton) & m_uPreviousMouseState) == 0);
}

bool Events::is_button_pressed(const Uint32 uButton) const
{
	return (SDL_BUTTON(uButton) & m_uCurrentMouseState) != 0;
}

bool Events::is_button_released (const Uint32 uButton) const
{
	return ((SDL_BUTTON(uButton) & m_uCurrentMouseState) == 0) && ((SDL_BUTTON(uButton) & m_uPreviousMouseState) != 0);
}

std::tuple<int, int> Events::get_mouse() {

	std::tuple<int, int> converted;
	converted = std::make_tuple((int)std::get<0>(this->mouse) / 64, (int)std::get<1>(this->mouse) / 64);
	return converted;

}