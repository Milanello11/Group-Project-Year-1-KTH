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
    bool activeSnowball;
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
    pCharacter->activeSnowball = false;
    
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
    switch (number)
    {
    case 0:
        pCharacter->xStart = pCharacter->x_pos = pCharacter->characterRect.x = 100;
        pCharacter->yStart = pCharacter->y_pos = pCharacter->characterRect.y = 100;
        break;
    case 1:
        pCharacter->xStart = pCharacter->x_pos = pCharacter->characterRect.x = 700;
        pCharacter->yStart = pCharacter->y_pos = pCharacter->characterRect.y = 100;
        break;
    case 2:
        pCharacter->xStart = pCharacter->x_pos = pCharacter->characterRect.x = 100;
        pCharacter->yStart = pCharacter->y_pos = pCharacter->characterRect.y = 600;
        break;
    case 3:
        pCharacter->xStart = pCharacter->x_pos = pCharacter->characterRect.x = 700;
        pCharacter->yStart = pCharacter->y_pos = pCharacter->characterRect.y = 600;
        break;
    default:
        break;
    }

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
    if(getCollision(pCharacter->x_pos, pCharacter->y_pos + pCharacter->characterRect.h)){

        pCharacter->x_pos = pCharacter->characterRect.x;
        pCharacter->y_pos = pCharacter->characterRect.y;
    }
    pCharacter->characterRect.x = pCharacter->x_pos;
    pCharacter->characterRect.y = pCharacter->y_pos;
}

void drawCharacter(Character *pCharacter){
    SDL_RenderCopy(pCharacter->pRenderer, pCharacter->pTexture, NULL, &(pCharacter->characterRect));
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
bool getActiveSnowball(Character *pCharacter){
    return (pCharacter->activeSnowball);
}
void updateActiveSnowball(Character *pCharacter){
    if(pCharacter->activeSnowball){
        pCharacter->activeSnowball = false;
        return;
    }
    pCharacter->activeSnowball = true;
}
void updateCharacterWithRecievedData(Character *pCharacter, CharacterData *pCharacterData){
    pCharacter->x_vel = pCharacterData->x_vel;
    pCharacter->y_vel = pCharacterData->y_vel;
    pCharacter->x_pos = pCharacterData->x_pos;
    pCharacter->y_pos = pCharacterData->y_pos;
}
void getCharacterSendData(Character *pCharacter, CharacterData *pCharacterData){
    pCharacterData->x_vel = pCharacter->x_vel;
    pCharacterData->y_vel = pCharacter->y_vel;
    pCharacterData->x_pos = pCharacter->x_pos;
    pCharacterData->y_pos = pCharacter->y_pos;
}