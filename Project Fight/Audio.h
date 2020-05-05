#pragma once

#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class Audio
{

public:

	Mix_Music* load_music(std::string path);

	Mix_Chunk* load_sound(std::string path);

	void play_music(Mix_Music* music);
	void play_sound(Mix_Chunk* chunk);

private:


};

#endif