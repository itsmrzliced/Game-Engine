#include "Engine.h"

Engine::Engine(int width, int height, std::string title) {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

		std::cout << "SDL couldn't be initialized: " << SDL_GetError() << std::endl; 

		return;

	}

	if (IMG_Init(IMG_INIT_PNG) < 0) {

		std::cout << "SDL_image couldn't be initialized: " << IMG_GetError() << std::endl; 

		return; 

	}

	if (TTF_Init() < 0) {

		std::cout << "SDL_ttf couldn't be initialized: " << TTF_GetError() << std::endl; 

		return; 

	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {

		std::cout << "SDL_mixer couldn't be initialized: " << Mix_GetError() << std::endl; 
		
		return; 

	}

	this->width = width;
	this->height = height;
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

	if (window == NULL) {

		std::cout << "SDL_Window couldn't be initialized: " << SDL_GetError() << std::endl; 

		return; 

	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL) {

		std::cout << "SDL_Renderer couldn't be initialized: " << SDL_GetError() << std::endl;

		return;

	}

}

SDL_Window* Engine::get_window() {

	return window; 

}

SDL_Renderer* Engine::get_renderer() {

	return renderer; 

}

int Engine::get_width() {

	return width; 

}

int Engine::get_height() {

	return height;

}

bool Engine::get_running() {

	return running;

}

void Engine::close() {

	running = false; 

	return; 

}

bool Engine::collision(SDL_Rect rect1, SDL_Rect rect2) {

	if (rect1.x < rect2.x + rect2.w && rect1.x + rect1.w > rect2.x&& rect1.y < rect2.y + rect2.h && rect1.y + rect1.h > rect2.y) {

		return true;

	}

	else {

		return false;

	}

}


bool Engine::collision_range(SDL_Rect rect1, SDL_Rect rect2, int x, int y) {

	if (rect1.x < rect2.x + rect2.w + x && rect1.x + rect1.w > rect2.x - x && rect1.y < rect2.y + rect2.h + y && rect1.y + rect1.h > rect2.y - y) {

		return true;

	}
	else
		return false;

}

Engine::~Engine() {

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();

}