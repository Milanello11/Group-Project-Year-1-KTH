#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "snowball.h"


struct snowball{
    float x_pos, y_pos, x_vel, y_vel;
    int window_width, window_height;

    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect snowballRect;
};


Snowball *createSnowball(SDL_Renderer *pRenderer, int window_width , int window_height){
    Snowball *pSnowball = malloc(sizeof(struct snowball));
    pSnowball->x_vel = 1;
    pSnowball->y_vel = 0;
    pSnowball->window_width = window_width;
    pSnowball->window_height = window_height;

    SDL_Surface *pSurface = IMG_Load("resources/Snowball.png");
    if (!pSurface){
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    pSnowball->pRenderer = pRenderer;
    pSnowball->pTexture = SDL_CreateTextureFromSurface(pRenderer , pSurface);
    SDL_FreeSurface(pSurface);
    if (!pSnowball->pTexture){
        printf("Error: %s\n" , SDL_GetError());
        return NULL;
    }
    SDL_QueryTexture(pSnowball->pTexture, NULL, NULL, &(pSnowball->snowballRect.w), &(pSnowball->snowballRect.h));
    pSnowball->snowballRect.w /= 4;
    pSnowball->snowballRect.h /= 4;
    pSnowball->x_pos = pSnowball->snowballRect.x = (window_width / 2);
    pSnowball->y_pos = pSnowball->snowballRect.y = (window_height / 2);
}

void updateSnowball(Snowball *pSnowball){ 
    pSnowball->x_pos += pSnowball->x_vel;
    pSnowball->y_pos += pSnowball->y_vel;

    if(pSnowball->x_pos > (pSnowball->window_width - pSnowball->snowballRect.w)){
        pSnowball->x_vel = -1;
    }
    if(pSnowball->x_pos < 0){
        pSnowball->x_vel = 1;
    }

    pSnowball->snowballRect.x = pSnowball->x_pos;
    pSnowball->snowballRect.y = pSnowball->y_pos;
}

void drawSnowball(Snowball *pSnowball){
    SDL_RenderCopyEx(pSnowball->pRenderer, pSnowball->pTexture, NULL, &(pSnowball->snowballRect), 0, NULL, SDL_FLIP_NONE);
}

void destroySnowball(Snowball *pSnowball){
    SDL_DestroyTexture(pSnowball->pTexture);
    free(pSnowball);
}
