#include "Audio.h"

Mix_Music* Audio::load_music(std::string path) {

	Mix_Music* music = Mix_LoadMUS(path.c_str());

	return music;

}

Mix_Chunk* Audio::load_sound(std::string path) {

	Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());

	return chunk;

}

void Audio::play_music(Mix_Music* music) {

	if (Mix_PlayingMusic() == 0) {

		Mix_PlayMusic(music, -1);

	}

	else {

		Mix_HaltMusic();

		Mix_PlayMusic(music, -1);

	}

}

void Audio::play_sound(Mix_Chunk* chunk) {

	Mix_PlayChannel(-1, chunk, 0);

}