#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "character.h"

struct character{
    float x_pos, y_pos, x_vel, y_vel;
    int window_width , window_height;

    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect characterRect;
}

Character *createCharacter(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height){
    Character *pCharacter = malloc(sizeof(struct character));
    pCharacter->vx = 0;
    pCharacter->vy = 0;
    pCharacter->window_width = window_width;
    pCharacter->window_height = window_height;
    SDL_Surface *pSurface = IMG_Load("resources/Ship.png");
    if (!pSurface){
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    pCharacter->pRenderer = pRenderer;
    pCharacter->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if (!pCharacter->pTexture){
        printf("Error: %s\n" , SDL_GetError());
        return NULL;
    }
    SDL_QueryTexture(pCharacter->pTexture,NULL,NULL,&(pCharacter->characterRect.w) , &(pCharacter->characterRect.h));
    pCharacter->characterRect.w /= 4;
    pCharacter->characterRect.h /= 4;
    pCharacter->x_pos = x - (pCharacter->characterRect.w/2);
    pCharacter->y_pos = y - (pCharacter->characterRect.h/2);

    return pCharacter;
}

void updateCharacter(Character *pCharacter){
    pCharacter->x_pos += pCharacter->x_vel;
    pCharacter->y_pos += pCharacter->y_vel;
}

void drawCharacter(Character *pCharacter){
    SDL_RenderCopyEx(pCharacter->pRenderer , pCharacter->pTexture , &(pCharacter->characterRect) , NULL , NULL ,SDL_FLIP_NONE);
}

void destroyCharacter(Character *pCharacter)
{
    SDL_DestroyTexture(pCharacter->pTexture);
    free(pCharacter);
}