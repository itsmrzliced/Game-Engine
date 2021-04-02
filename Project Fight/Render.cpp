#include "Render.h"

Render::Render(Engine* engine) {

	SDL_SetRenderDrawColor(engine->get_renderer(), 0, 0, 0, 255);

}

void Render::draw(Engine* engine, Board* board) {

	SDL_RenderClear(engine->get_renderer());

	
	// Draw 
	board->draw(engine);
	for (int i = 0; i < engine->get_pieces().size(); i++) {
		SDL_SetTextureBlendMode(engine->get_pieces()[i]->texture, SDL_BLENDMODE_BLEND);
		if (engine->get_pieces()[i]->selected) {

			SDL_SetTextureAlphaMod(engine->get_pieces()[i]->texture, 128);

		}
		else {

			SDL_SetTextureAlphaMod(engine->get_pieces()[i]->texture, 255);

		}
		
	}

	engine->draw();
	

	SDL_RenderPresent(engine->get_renderer());

}

SDL_Texture* Render::load_texture(Engine* engine, std::string path) {

	SDL_Surface* surface = IMG_Load(path.c_str());

	if (surface == nullptr) {

		std::cout << "Image doesn't exist!" << std::endl; 

	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(engine->get_renderer(), surface);

	return texture; 

}

TTF_Font* Render::load_font(Engine* engine, std::string path, int size) {

	TTF_Font* font = TTF_OpenFont(path.c_str(), size);

	return font; 

}

SDL_Texture* Render::load_text(Engine* engine, TTF_Font* font, std::string text, SDL_Color color) {

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(engine->get_renderer(), surface);

	return texture; 

}

SDL_Rect Render::load_text_dimensions(Engine* engine, SDL_Texture* texture) {

	int texW = 0;
	int texH = 0; 

	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

	SDL_Rect rect = { 0, 0, texW, texH };

	return rect; 


}

Render::~Render() {



}