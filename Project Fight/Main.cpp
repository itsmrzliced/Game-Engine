#include <iostream>

#include "Engine.h"
#include "Events.h"
#include "Render.h"
#include "Audio.h"

int main(int argc, char** argv) {

	const int FPS = 60;
	const int frame_delay = 1000 / FPS;

	Uint32 frame_start;
	int frame_time;

	Engine* engine = new Engine(600, 600, "Project Fight");
	Events* events = new Events();
	Render* render = new Render(engine);
	Audio* audio = new Audio();

	while (engine->get_running()) {

		frame_start = SDL_GetTicks();

		render->draw(engine);

		events->update(engine);

		frame_time = SDL_GetTicks() - frame_start;

		if (frame_delay > frame_time) {

			SDL_Delay(frame_delay - frame_time);

		}

	}

	delete events;
	delete render; 
	delete engine; 
	delete audio; 

	return 0;

}