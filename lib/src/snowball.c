#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "snowball.h"
#include <stdbool.h>


struct snowball{
    float x_pos, y_pos, x_vel, y_vel;
    int window_width, window_height, isOnScreen, owner;

    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect snowballRect;
};


Snowball *createSnowball(SDL_Renderer *pRenderer, int window_width , int window_height){
    Snowball *pSnowball = malloc(sizeof(struct snowball));
    pSnowball->x_vel = pSnowball->y_vel = 0;
    pSnowball->isOnScreen = 0;
    pSnowball->window_width = window_width;
    pSnowball->window_height = window_height;
    pSnowball->owner = -1;

    SDL_Surface *pSurface = IMG_Load("../lib/resources/Snowball.png");
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
    pSnowball->snowballRect.w /= 3;
    pSnowball->snowballRect.h /= 3;
    pSnowball->x_pos = pSnowball->snowballRect.x = -100;
    pSnowball->y_pos = pSnowball->snowballRect.y = -100;
    return pSnowball;
}

void updateSnowball(Snowball *pSnowball){ 

    
    if (pSnowball->x_pos > pSnowball->window_width || pSnowball->x_pos < 0 - pSnowball->snowballRect.w || pSnowball->y_pos > pSnowball->window_height || pSnowball->y_pos < 0 - pSnowball->snowballRect.h){
        pSnowball->x_vel = 0;
        pSnowball->y_vel = 0;
        pSnowball->isOnScreen = 0;
        pSnowball->owner = -1;
    }
    pSnowball->x_pos += pSnowball->x_vel;
    pSnowball->y_pos += pSnowball->y_vel;

    pSnowball->snowballRect.x = pSnowball->x_pos;
    pSnowball->snowballRect.y = pSnowball->y_pos;
}

void drawSnowball(Snowball *pSnowball){
    SDL_RenderCopyEx(pSnowball->pRenderer, pSnowball->pTexture, NULL, &(pSnowball->snowballRect), 0, NULL, SDL_FLIP_NONE);
}

void destroySnowball(Snowball *pSnowball){
    SDL_DestroyTexture(pSnowball->pTexture);
    free(pSnowball);
    pSnowball = NULL;
}           
void startSnowball(Snowball *pSnowball, int startX, int startY, int directionIndex){
    pSnowball->isOnScreen = 1;
    switch (directionIndex){
        case 0: 
            pSnowball->x_vel = 0;
            pSnowball->y_vel = -SNOWBALLSPEED;
            pSnowball->x_pos = startX;
            pSnowball->y_pos = startY - 1;
        break;
        case 1: 
            pSnowball->x_vel = SNOWBALLSPEED;
            pSnowball->y_vel = 0;
            pSnowball->x_pos = startX + 11;
            pSnowball->y_pos = startY;
        break;
        case 2: 
            pSnowball->x_vel = 0;
            pSnowball->y_vel = SNOWBALLSPEED;
            pSnowball->x_pos = startX;
            pSnowball->y_pos = startY + 15;
        break;
        case 3: 
            pSnowball->x_vel = -SNOWBALLSPEED;
            pSnowball->y_vel = 0;
            pSnowball->x_pos = startX - 1;
            pSnowball->y_pos = startY;
        break;
    }
}
int getOnScreenIndex(Snowball *pSnowball){
    return (pSnowball->isOnScreen);
}

void getSnowballSendData(Snowball *pSnowball, SnowballData *pSnowballData){
    pSnowballData->x_vel = pSnowball->x_vel;
    pSnowballData->y_vel = pSnowball->y_vel;
    pSnowballData->x_pos = pSnowball->x_pos;
    pSnowballData->y_pos = pSnowball->y_pos;
}
void updateSnowballWithRecievedData(Snowball *pSnowball, SnowballData *pSnowballData){
    pSnowball->x_vel = pSnowballData->x_vel;
    pSnowball->y_vel = pSnowballData->y_vel;
    pSnowball->x_pos = pSnowballData->x_pos;
    pSnowball->y_pos = pSnowballData->y_pos;
}
void setSnowballOwner(Snowball *pSnowball, int number){
    pSnowball->owner = number;
}
int getSnowballOwner(Snowball *pSnowball){
    return (pSnowball->owner);
}
SDL_Rect getSnowballRect(Snowball *pSnowball){
    return (pSnowball->snowballRect);
}