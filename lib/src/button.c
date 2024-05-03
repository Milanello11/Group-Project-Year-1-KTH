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

Button *createButton(SDL_Renderer *pRenderer, int x, int y, int w, int h){

    Button *pButton = malloc(sizeof(struct button));
    SDL_Surface *pSurface = IMG_Load("../lib/resources/Buttons.png");
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

int hover(int x, int y, Button *pButton){
    
    if (x >= pButton->buttonDesRect.x && x <= pButton->buttonDesRect.x + pButton->buttonDesRect.w && y >= pButton->buttonDesRect.y && y <= pButton->buttonDesRect.y + pButton->buttonDesRect.h){
        return 1;
    }

    return 0;
}

void drawButton(Button *pButton , int index){

    if (index == 1){
        pButton->buttonSrcRect.x = 400;
    }
    else if (index == 2){
        pButton->buttonSrcRect.x = 800;
    }
    else if (index == 3){
        pButton->buttonSrcRect.x = 200;
    }
    else if (index == 4){
        pButton->buttonSrcRect.x = 600;
    }
    else if (index == 5){
        pButton->buttonSrcRect.x = 1000;
    }
    else {
        pButton->buttonSrcRect.x = 0;
    }
    pButton->buttonSrcRect.y = 0;
    pButton->buttonSrcRect.w = 200;
    pButton->buttonSrcRect.h = 105;

    SDL_RenderCopy(pButton->pRenderer, pButton->pTexture, &(pButton->buttonSrcRect), &(pButton->buttonDesRect));
}

void destroyButton(Button *pButton){
    SDL_DestroyTexture(pButton->pTexture);
    free(pButton);
}
void setDesRect(Button *pButton, int index){

    if (index == 0){
        pButton->buttonDesRect.x = 200;
        pButton->buttonDesRect.y = 200;
        pButton->buttonDesRect.w = 400;
        pButton->buttonDesRect.h = 155;
    }
    else if (index == 1){
        pButton->buttonDesRect.x = 300;
        pButton->buttonDesRect.y = 400;
        pButton->buttonDesRect.w = 200;
        pButton->buttonDesRect.h = 105;
    }
    else if(index == 2){
        pButton->buttonDesRect.x = 580;
        pButton->buttonDesRect.y = 650;
        pButton->buttonDesRect.w = 200;
        pButton->buttonDesRect.h = 105;
    }
    else if(index == 3){
        pButton->buttonDesRect.x = 300;
        pButton->buttonDesRect.y = 660;
        pButton->buttonDesRect.w = 200;
        pButton->buttonDesRect.h = 105;
    }
}