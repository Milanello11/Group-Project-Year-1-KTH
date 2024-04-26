#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "button.h"

struct button{

    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;

    SDL_Rect buttonSrcRect;
    SDL_Rect buttonDesRect;
    bool buttonIsClicked;

};

Button *initButton(SDL_Renderer *pRenderer, int x, int y, int w, int h){

    Button *pButton = malloc(sizeof(struct button));
    SDL_Surface *pSurface = IMG_Load("../lib/resources/startButton.png");
    if(!pSurface){
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    pButton->pRenderer = pRenderer;
    pButton->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if(!pButton->pTexture){
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }

    return pButton;
}

int setSrcRectX(int x, int y){
    if (x >= 300 && x <= 500 && y >= 200 && y <= 305){
        return 1;
    }






    return 0;
}

void drawButton(Button *pButton , int index){

    if (index == 1){
        pButton->buttonSrcRect.x = 200;
    }
    else {
        pButton->buttonSrcRect.x = 0;
    }
    pButton->buttonSrcRect.y = 0;
    pButton->buttonSrcRect.w = 200;
    pButton->buttonSrcRect.h = 105;

    pButton->buttonDesRect.x = 300;
    pButton->buttonDesRect.y = 200;
    pButton->buttonDesRect.w = 200;
    pButton->buttonDesRect.h = 105;



    SDL_RenderCopy(pButton->pRenderer, pButton->pTexture, &(pButton->buttonSrcRect), &(pButton->buttonDesRect));
}

void destroyButton(Button *pButton){
    SDL_DestroyTexture(pButton->pTexture);
    free(pButton);
}