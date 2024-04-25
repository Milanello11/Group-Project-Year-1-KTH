#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <stdio.h>
#include "menu.h"

struct menu{
    int window_w, window_h;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
};

Menu *createMenuBackground(SDL_Renderer *pRenderer, int window_w, int window_h){
    Menu *pMenuBackground = malloc(sizeof(struct menu));
    pMenuBackground->pRenderer = pRenderer;
    SDL_Surface *pSurface = IMG_Load("../lib/resources/Bakgrund.png");
    if(!pSurface){
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    pMenuBackground->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if (!pMenuBackground->pTexture){
        printf("Error: %s", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(pSurface);

    pMenuBackground->window_w = window_w;
    pMenuBackground->window_h = window_h;

    return pMenuBackground;

}

void renderMenuBackground(Menu *pMenuBackground){
    SDL_RenderCopy(pMenuBackground->pRenderer, pMenuBackground->pTexture, NULL, NULL);
}