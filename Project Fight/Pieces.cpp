#include "Pieces.h"

Pieces::Pieces(std::string name, std::string color, std::tuple<int, int> pos, Engine* engine, int score) {

	this->name = name; 
	this->color = color;
	this->pos = pos;
	
	this->coords = get_coords(this->pos);
	int srcw = 64;
	int srch = 64;
	this->score = score;
	this->srcrect = {};
	if (this->name == "Pawn" && this->color == "White") {

		this->srcrect = { srcw * 5, srch * 1, srcw, srch };

	}

	if (this->name == "Pawn" && this->color == "Black") {

		this->srcrect = { srcw * 5, 0, srcw, srch };

	}

	if (this->name == "Rook" && this->color == "White") {

		this->srcrect = { srcw * 4, srch, srcw, srch };

	}

	if (this->name == "Rook" && this->color == "Black") {

		this->srcrect = { srcw * 4, 0, srcw, srch };

	}

	if (this->name == "Knight" && this->color == "White") {

		this->srcrect = { srcw * 3, srch, srcw, srch };

	}

	if (this->name == "Knight" && this->color == "Black") {

		this->srcrect = { srcw * 3, 0, srcw, srch };

	}

	if (this->name == "Bishop" && this->color == "White") {

		this->srcrect = { srcw * 2, srch, srcw, srch };

	}

	if (this->name == "Bishop" && this->color == "Black") {

		this->srcrect = { srcw * 2, 0, srcw, srch };

	}

	if (this->name == "Queen" && this->color == "White") {

		this->srcrect = { srcw, srch, srcw, srch };

	}

	if (this->name == "Queen" && this->color == "Black") {

		this->srcrect = { srcw, 0, srcw, srch };

	}

	if (this->name == "King" && this->color == "White") {

		this->srcrect = { 0, srch, srcw, srch };

	}

	if (this->name == "King" && this->color == "Black") {

		this->srcrect = { 0, 0, srcw, srch };

	}

	this->dstrect = {std::get<0>(this->coords), std::get<1>(this->coords), this->w, this->h};
	SDL_Surface* surface = IMG_Load("res/Chess/sheet.png");
	this->texture = SDL_CreateTextureFromSurface(engine->get_renderer(), surface);
	SDL_FreeSurface(surface);

}

Pieces::~Pieces() {



}



std::tuple<int, int> Pieces::get_coords(std::tuple<int, int> pos) {

	std::tuple<int, int> converted;
	converted = std::make_tuple(std::get<0>(pos) * 64, std::get<1>(pos) * 64);

	return converted;

}

std::tuple<int, int> Pieces::get_pos(std::tuple<int, int> coords) {

	std::tuple<int, int> converted;
	converted = std::make_tuple(std::get<0>(coords) / 64, std::get<1>(coords) / 64);

	return converted;

}

std::tuple<int, int> Pieces::return_pos() {

	return this->pos;

}

std::string Pieces::get_name() {

	return this->name;

}

std::string Pieces::get_color() {

	return this->color;

}