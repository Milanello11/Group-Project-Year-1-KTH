#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "snowball.h"


struct snowball{
    float x_pos, y_pos, x_vel, y_vel;
    int window_width, window_height;

    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect snowballRect;
};

Snowball *createSnowball(int x , int y, int x_vel , int y_vel , SDL_Renderer *pRenderer, int window_w, int window_h){
    Snowball *pSnowball = malloc(sizeof(struct snowball));
    pSnowball->x_vel = x_vel;
    pSnowball->y_vel = y_vel;
    pSnowball->window_width = window_w;
    pSnowball->window_height = window_h;
    SDL_Surface *pSurface = IMG_Load("resources/Snowball.png");
    if (!pSurface){
        printf("Error: %s \n", SDL_GetError());
        return NULL;
    }
    pSnowball->pRenderer = pRenderer;
    pSnowball->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if (!pSnowball->pTexture){
        printf("Error: %s\n" , SDL_GetError());
        return NULL;
    }
    SDL_QueryTexture(pSnowball->pTexture,NULL,NULL,&(pSnowball->snowballRect.w) , &(pSnowball->snowballRect.h));
    pSnowball->snowballRect.w /= 4;
    pSnowball->snowballRect.h /= 4;
    pSnowball->x_pos = x ;
    pSnowball->y_pos = y ;

    return pSnowball;
}

void destroySnowball(Snowball *pSnowball){
    SDL_DestroyTexture(pSnowball->pTexture);
    free(pSnowball);
}

void updateSnowball(Snowball *pSnowball){
    pSnowball->x_pos += pSnowball->x_vel;
    pSnowball->y_pos += pSnowball->y_vel;

    pSnowball->snowballRect.x = pSnowball->x_pos;
    pSnowball->snowballRect.y = pSnowball->y_pos;
}

void drawSnowball(Snowball *pSnowball){
    SDL_RenderCopy(pSnowball->pRenderer, pSnowball->pTexture, NULL, &(pSnowball->snowballRect));
}