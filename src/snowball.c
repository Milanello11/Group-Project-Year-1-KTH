#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "snowball.h"


struct snowballImage{
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
};

struct snowball{
    float x_pos, y_pos, x_vel, y_vel;
    int window_width, window_height;

    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect snowballRect;
};

SnowballImage *createSnowballImage(SDL_Renderer *pRenderer){
    SnowballImage *pSnowballImage = NULL;
    if (pSnowballImage == NULL){
        pSnowballImage = malloc(sizeof(struct snowballImage));
        SDL_Surface *pSurface = IMG_Load("resources/Snowball.png");
        if(!pSurface){
            printf("Error: %s\n",SDL_GetError());
            return NULL;
        }
        pSnowballImage->pRenderer = pRenderer;
        pSnowballImage->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
        SDL_FreeSurface(pSurface);
        if(!pSnowballImage->pTexture){
            printf("Error: %s\n",SDL_GetError());
            return NULL;
        }
    }
    return pSnowballImage;
}

Snowball *createSnowball(SnowballImage *pSnowballImage, int window_width , int window_height){
    Snowball *pSnowball = malloc(sizeof(struct snowball));
    pSnowball->pRenderer = pSnowballImage->pRenderer;
    pSnowball->pTexture = pSnowballImage->pTexture;
    pSnowball->window_width = window_width;
    pSnowball->window_height = window_height;
    SDL_QueryTexture(pSnowballImage->pTexture, NULL, NULL, &(pSnowball->snowballRect.w), &(pSnowball->snowballRect.h));
}

void destroySnowball(Snowball *pSnowball){
    free(pSnowball);
}

void destroySnowballImage(SnowballImage *pSnowballImage){
    SDL_DestroyTexture(pSnowballImage->pTexture);
    free(pSnowballImage);
}

void drawSnowball(Snowball *pSnowball, SDL_Renderer *pRenderer){
    printf("Johan snear\n");
    SDL_RenderCopy(pSnowball->pRenderer, pSnowball->pTexture, NULL, NULL);  //här är något knasigt 
    printf("Johan snear\n");
}
/*void updateSnowball(Snowball *pSnowball){ //vi kommer in här men kan inte utföra något
    printf("Elias snea\n");
    //pSnowball->snowballRect.x = pSnowball->x_pos = 0;
    //pSnowball->snowballRect.y = pSnowball->y_pos = 0;
}*/