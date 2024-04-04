#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argv, char** args)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Snomos The Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, 0);
    //SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    if(!window_surface)
    {
        printf("Failed to get the surface from the window\n");
        return -1;
    }

    bool isRunning = true;
    SDL_Event event;
    SDL_Surface *pImage = IMG_Load("resources/Bakgrund.png");

    if(!pImage)
    {
        printf("Failed to load image\n");
        return -1;
    }

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    isRunning = false;
                }
            }
            SDL_BlitSurface(pImage, NULL, window_surface, NULL);
            SDL_UpdateWindowSurface(window);
        }

        //SDL_RenderClear(renderer);
        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        //SDL_RenderPresent(renderer);
    }

    //SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(window_surface);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
