#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>

#define SPEED 400

int main (int argument, char* arguments[]){

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Snomos", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    
    SDL_Surface *backgroundSurface = IMG_Load("resources/Bakgrund.png");
    SDL_Surface *characterSurface = IMG_Load("resources/Ship.png");

    if (!backgroundSurface || !characterSurface){
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_Texture *characterTexture = SDL_CreateTextureFromSurface(renderer, characterSurface);
    SDL_FreeSurface(backgroundSurface);
    SDL_FreeSurface(characterSurface);

    if (!backgroundTexture || !characterTexture){
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool active = true;
    bool up, down, left, right;
    up = down = left = right = false;
    SDL_Event event;
    SDL_Rect characterRect;
    SDL_QueryTexture(characterTexture, NULL, NULL, &characterRect.w, &characterRect.h);
    characterRect.x = windowWidth/2;
    characterRect.y = windowHeight/2;

    float characterX = (windowWidth - characterRect.w)/2;
    float characterY = (windowHeight - characterRect.h)/2;
    float characterXVelocity = 0;
    float characterYVelocity = 0; //pixels per second

    while (active){
        while (SDL_PollEvent(&event)){

            switch (event.type){

                case SDL_QUIT: active = false;
                break;

                case SDL_KEYDOWN:

                    if (event.key.keysym.sym == SDLK_ESCAPE){
                        active = false;
                    }

                    switch (event.key.keysym.scancode){
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            up = true;
                        break;

                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            down = true;
                        break;

                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            left = true;
                        break;

                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            right = true;
                        break;
                    }
                break;

                case SDL_KEYUP:
                    switch(event.key.keysym.scancode){
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            up=false;
                        break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            left=false;
                        break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            down=false;
                        break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            right=false;
                        break;
                    }
                    break;


            }
        }

        characterXVelocity = characterYVelocity = 0;
        if(up && !down) characterYVelocity = -SPEED;
        if(down && !up) characterYVelocity = SPEED;
        if(left && !right) characterXVelocity = -SPEED;
        if(right && !left) characterXVelocity = SPEED;
        characterX += characterXVelocity/60;//60 frames/s
        characterY += characterYVelocity/60;
        if(characterX<0) characterX=0;
        if(characterY<0) characterY=0;
        if(characterX>windowWidth-characterRect.w) characterX = windowWidth-characterRect.w;
        if(characterY>windowHeight-characterRect.h) characterY = windowHeight-characterRect.h;
        characterRect.x = characterX;
        characterRect.y = characterY;

        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}


