#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "character.h"

struct character{
    float x_pos, y_pos, x_vel, y_vel;
    int window_width , window_height;

    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect characterRect;
};

Character *createCharacter(int x, int y, SDL_Renderer *pRenderer, int window_w, int window_h){
    Character *pCharacter = malloc(sizeof(struct character));
    pCharacter->x_vel = 0;
    pCharacter->y_vel = 0;
    pCharacter->window_width = window_w;
    pCharacter->window_height = window_h;
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
    pCharacter->characterRect.x = pCharacter->x_pos;
    pCharacter->characterRect.y = pCharacter->y_pos;
}

void drawCharacter(Character *pCharacter){
    SDL_RenderCopy(pCharacter->pRenderer , pCharacter->pTexture ,NULL, &(pCharacter->characterRect));
}

void destroyCharacter(Character *pCharacter)
{
    SDL_DestroyTexture(pCharacter->pTexture);
    free(pCharacter);
}