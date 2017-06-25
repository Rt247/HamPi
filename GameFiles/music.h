//
// Created by AthiDharma on 19/06/2017.
//

#include <SDL2/SDL_mixer.h>

#ifndef GAME_MUSIC_H
#define GAME_MUSIC_H

void initialiseSDLMixer();
void clearUpSDLMusic();
Mix_Music *helperMusic;
Mix_Music *introMusic;
Mix_Music *endMusic;
Mix_Music *levelMusic1;
Mix_Music *levelMusic2;
Mix_Music *levelMusic3;
Mix_Music *levelMusic4;
Mix_Music *levelMusic5;

void playMusic(Mix_Music *music);
void playLevelMusic();
void stopMusic(int time);

#endif //GAME_MUSIC_H
