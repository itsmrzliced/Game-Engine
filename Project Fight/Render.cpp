#include "Render.h"

Render::Render(Engine* engine) {

	SDL_SetRenderDrawColor(engine->get_renderer(), 0, 0, 0, 255);

}

void Render::draw(Engine* engine) {

	SDL_RenderClear(engine->get_renderer());

	// Draw 

	SDL_RenderPresent(engine->get_renderer());

}

SDL_Texture* Render::load_texture(Engine* engine, std::string path) {

	SDL_Surface* surface = IMG_Load(path.c_str());

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