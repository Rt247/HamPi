//
// Created by AthiDharma on 19/06/2017.
//

#include <stdio.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include "music.h"

void initialiseSDLMixer() {
  //Initialize SDL_mixer
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
    perror("Mixer failed to initialise");
  }

  helperMusic = Mix_LoadMUS("../Music/Batman.mp3");
  if (helperMusic == NULL) {
    printf("DIDNT LOAD FILE batman music");
  }
  introMusic = Mix_LoadMUS("../Music/Arpanauts.mp3");
  if (introMusic == NULL) {
    printf("DIDNT LOAD FILE intro music ");
  }
  levelMusic1 = Mix_LoadMUS("../Music/Eric_Skiff_-_01_-_A_Night_Of_Dizzy_Spells.mp3");
  if (levelMusic1 == NULL) {
    printf("DIDNT LOAD FILE intro music ");
  }
  levelMusic2 = Mix_LoadMUS("../Music/Eric_Skiff_-_08_-_Ascending.mp3");
  if (levelMusic2 == NULL) {
    printf("DIDNT LOAD FILE intro music ");
  }
  levelMusic3 = Mix_LoadMUS("../Music/Eric_Skiff_-_03_-_Chibi_Ninja.mp3");
  if (levelMusic3 == NULL) {
    printf("DIDNT LOAD FILE intro music ");
  }
  levelMusic4 = Mix_LoadMUS("../Music/Eric_Skiff_-_02_-_Underclocked_underunderclocked_mix.mp3");
  if (levelMusic4 == NULL) {
    printf("DIDNT LOAD FILE intro music ");
  }
  levelMusic5 = Mix_LoadMUS
      ("../Music/Eric_Skiff_-_05_-_Come_and_Find_Me.mp3");
  if (levelMusic5 == NULL) {
    printf("DIDNT LOAD FILE intro music ");
  }
  endMusic = Mix_LoadMUS
      ("../Music/endTheme.mp3");
  if (endMusic == NULL) {
    printf("DIDNT LOAD FILE intro music ");
  }
}

void clearUpSDLMusic() {
  Mix_FreeMusic(helperMusic);
  Mix_FreeMusic(introMusic);
  Mix_FreeMusic(levelMusic1);
  Mix_FreeMusic(levelMusic2);
  Mix_FreeMusic(levelMusic3);
  Mix_FreeMusic(levelMusic4);
  Mix_FreeMusic(levelMusic5);
  Mix_FreeMusic(endMusic);
  Mix_CloseAudio();
}

void playMusic(Mix_Music *music) {
  Mix_PlayMusic(music, -1);
}

void playLevelMusic() {
  int randSong = (rand() % 5) + 1;
  switch (randSong) {
    case 1:
      Mix_PlayMusic(levelMusic1, -1);
      break;
    case 2:
      Mix_PlayMusic(levelMusic2, -1);
      break;
    case 3:
      Mix_PlayMusic(levelMusic3, -1);
      break;
    case 4:
      Mix_PlayMusic(levelMusic4, -1);
      break;
    default:
      Mix_PlayMusic(levelMusic5, -1);
  }
}

void stopMusic(int time) {
  while (!Mix_FadeOutMusic(time) && Mix_PlayingMusic()) {
    SDL_Delay(100);
  }
}