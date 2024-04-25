#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "character.h"
#include "snowball.h"
#include "map.h"
#include "characterData.h"
#include <stdbool.h>
#define CHARACTERVELOCITY 3

struct character{
    float x_pos, y_pos, x_vel, y_vel , xStart , yStart;
    int window_width , window_height;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect characterRect;
};

Character *createCharacter(int number, SDL_Renderer *pRenderer, int window_w, int window_h){
    Character *pCharacter = malloc(sizeof(struct character));
    pCharacter->x_vel = 0;
    pCharacter->y_vel = 0;
    pCharacter->window_width = window_w;
    pCharacter->window_height = window_h;
    
    SDL_Surface *pSurface = IMG_Load("../lib/resources/Character.png");
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
    SDL_QueryTexture(pCharacter->pTexture, NULL, NULL, &(pCharacter->characterRect.w), &(pCharacter->characterRect.h));
    pCharacter->characterRect.w /= 5;
    pCharacter->characterRect.h /= 5;
    pCharacter->xStart = pCharacter->x_pos = pCharacter->characterRect.x = window_w*(number + 1) / 6 - pCharacter->characterRect.w/2;
    pCharacter->xStart = pCharacter->y_pos = pCharacter->characterRect.y = window_h/2-pCharacter->characterRect.h/2;

    return pCharacter;
}

void updateCharacter(Character *pCharacter){
    pCharacter->x_pos += pCharacter->x_vel;
    pCharacter->y_pos += pCharacter->y_vel;
    if(pCharacter->x_pos < 0){
        pCharacter->x_pos = 0;
    } 
    else if (pCharacter->x_pos > (pCharacter->window_width - pCharacter->characterRect.w)){
        pCharacter->x_pos = pCharacter->window_width - pCharacter->characterRect.w;
    } 
    if(pCharacter->y_pos < 0){
        pCharacter->y_pos = 0;
    }
    else if(pCharacter->y_pos > (pCharacter->window_height - pCharacter->characterRect.h)){ 
        pCharacter->y_pos = pCharacter->window_height - pCharacter->characterRect.h;
    }
    if(getCollision(pCharacter->x_pos, pCharacter->y_pos)){

        pCharacter->x_pos = pCharacter->characterRect.x;
        pCharacter->y_pos = pCharacter->characterRect.y;
    }
    if(getCollision(pCharacter->x_pos + pCharacter->characterRect.w, pCharacter->y_pos + pCharacter->characterRect.h)){

        pCharacter->x_pos = pCharacter->characterRect.x;
        pCharacter->y_pos = pCharacter->characterRect.y;
    }
    if(getCollision(pCharacter->x_pos + pCharacter->characterRect.w, pCharacter->y_pos)){

        pCharacter->x_pos = pCharacter->characterRect.x;
        pCharacter->y_pos = pCharacter->characterRect.y;
    }
    if(getCollision(pCharacter->x_pos + pCharacter->characterRect.w, pCharacter->y_pos)){

        pCharacter->x_pos = pCharacter->characterRect.x;
        pCharacter->y_pos = pCharacter->characterRect.y;
    }
    pCharacter->characterRect.x = pCharacter->x_pos;
    pCharacter->characterRect.y = pCharacter->y_pos;
}

void drawCharacter(Character *pCharacter){
    SDL_RenderCopyEx(pCharacter->pRenderer, pCharacter->pTexture, NULL, &(pCharacter->characterRect), 0, NULL, SDL_FLIP_NONE);
}

void destroyCharacter(Character *pCharacter){
    SDL_DestroyTexture(pCharacter->pTexture);
    free(pCharacter);
}

void characterTurnUp(Character *pCharacter){
    pCharacter->y_vel = -(CHARACTERVELOCITY);
    pCharacter->x_vel = 0;
}

void characterTurnDown(Character *pCharacter){
    pCharacter->y_vel = CHARACTERVELOCITY;
    pCharacter->x_vel = 0;
}

void characterTurnRight(Character *pCharacter){
    pCharacter->x_vel = CHARACTERVELOCITY;
    pCharacter->y_vel = 0;
}

void characterTurnLeft(Character *pCharacter){
    pCharacter->x_vel = -(CHARACTERVELOCITY);
    pCharacter->y_vel = 0;
}
void characterXStop(Character *pCharacter){
    pCharacter->x_vel = 0;
}
void characterYStop(Character *pCharacter){
    pCharacter->y_vel = 0;
}
bool isColliding(SDL_Rect a, SDL_Rect b){
    return (a.x > b.x + b.w || a.x + a.w < b.x || a.y > b.y + b.h || a.y + a.h < b.y);
}
int getPlayerXPos(Character *pCharacter){
    return (pCharacter->characterRect.x);
}
int getPlayerYPos(Character *pCharacter){
    return (pCharacter->characterRect.y);
}
void updateCharacterWithRecievedData(Character *pCharacter, CharacterData *pCharacterData){
    pCharacter->x_vel = pCharacterData->x_vel;
    pCharacter->y_vel = pCharacterData->y_vel;
    pCharacter->x_pos = pCharacterData->x_pos;
    pCharacter->y_pos = pCharacterData->y_pos;
    //updateSnowballWithRecievedData(pCharacter->pSnowball, &(pCharacterData->SnowballData));
}
void getCharacterSendData(Character *pCharacter, CharacterData *pCharacterData){
    pCharacterData->x_vel = pCharacter->x_vel;
    pCharacterData->y_vel = pCharacter->y_vel;
    pCharacterData->x_pos = pCharacter->x_pos;
    pCharacterData->y_pos = pCharacter->y_pos;
    //getSnowballSendData(pCharacter->pSnowball,&(pCharacterData->SnowballData));
}