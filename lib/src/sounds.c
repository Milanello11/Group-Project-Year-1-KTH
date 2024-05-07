#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include "sounds.h"

struct sounds{
    Mix_Music *gameplayMusic;
    Mix_Music *menuMusic;
    Mix_Chunk *buttonEffect;
    Mix_Chunk *hitEffect;
    Mix_Chunk *throwEffect;
};

Sounds *createSounds(){
    Sounds *pSounds = malloc(sizeof(struct sounds));

    pSounds->gameplayMusic = Mix_LoadMUS("../lib/resources/audio/gameplay_music.mp3");
    pSounds->menuMusic = Mix_LoadMUS("../lib/resources/audio/menu_music.mp3");
    pSounds->buttonEffect = Mix_LoadWAV("../lib/resources/audio/buttonEffect.wav");
    pSounds->hitEffect = Mix_LoadWAV("../lib/resources/audio/hitEffect.wav");
    pSounds->throwEffect = Mix_LoadWAV("../lib/resources/audio/throwEffect.wav");

    Mix_VolumeMusic(30);
    //Mix_VolumeChunk(pSounds->buttonEffect, 64);

    return pSounds;
}

void playGameplayMusic(Sounds *pSounds){
    if (Mix_PlayMusic(pSounds->gameplayMusic, -1) == -1) {
        printf("Failed to play gameplay music: %s\n", Mix_GetError());
    }
}

void playMenuMusic(Sounds *pSounds){
    if (Mix_PlayMusic(pSounds->menuMusic, -1) == -1) {
        printf("Failed to play background music: %s\n", Mix_GetError());
    }
}

void playButtonEffect(Sounds *pSounds){
    Mix_PlayChannel(-1,pSounds->buttonEffect, 0);
}

void playThrowEffect(Sounds *pSounds){
    Mix_PlayChannel(-1,pSounds->throwEffect, 0);
}

void playHitEffect(Sounds *pSounds){
    Mix_PlayChannel(-1,pSounds->hitEffect, 0);
}

void musicCleanup(Sounds *pSounds){
    Mix_FreeMusic(pSounds->gameplayMusic);
    Mix_FreeMusic(pSounds->menuMusic);
    Mix_FreeChunk(pSounds->buttonEffect);
    Mix_FreeChunk(pSounds->hitEffect);
    Mix_FreeChunk(pSounds->throwEffect);
    Mix_CloseAudio();
    free(pSounds);
}