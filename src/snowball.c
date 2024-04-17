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