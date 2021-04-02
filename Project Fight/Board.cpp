#include "Board.h"

Board::Board(Engine* engine, Render* render) {

	//this->texture = render->load_texture(engine, "res/Chess/board.png");
	this->dstrect = { 0, 0, engine->get_width(), engine->get_height() };

	for (int i = 0; i < 8; i++) {

		for (int j = 0; j < 8; j++) {

			this->grid.push_back(std::make_tuple(i, j));

		}

	}

	for (int i = 0; i < engine->get_pieces().size(); i++) {

		for (int j = 0; j < this->grid.size(); j++) {

			if (engine->get_pieces()[i]->return_pos() == this->grid[j]) {

				this->occupied_pos.push_back(this->grid[j]);

			}

		}

	}


}

Board::~Board() {

	//SDL_DestroyTexture(this->texture);

}

void Board::draw(Engine* engine) {

	//SDL_RenderCopy(engine->get_renderer(), this->texture, NULL, &this->dstrect);

	int c = -1;
	for (unsigned i = 0; i < 8; i++) {

		c *= -1;
		for (unsigned j = 0; j < 8; j++) {

			c *= -1;
			if (c == 1) {

				SDL_SetRenderDrawColor(engine->get_renderer(), 222, 184, 135, 255);

			}

			else {

				SDL_SetRenderDrawColor(engine->get_renderer(), 255, 248, 220, 255);

			}

			SDL_Rect rect = { i * 64, j * 64, 64, 64 };

			SDL_RenderFillRect(engine->get_renderer(), &rect);

		}

	}

}

std::vector<std::tuple<int, int>> Board::return_grid() {

	return this->grid;

}

std::vector<std::tuple<int, int>> Board::return_occupied() {

	return this->occupied_pos;

}