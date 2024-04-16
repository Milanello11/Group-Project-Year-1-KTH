#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>
#include "character.h"



typedef struct{
        SDL_Window *pWindow;
        SDL_Renderer *pRenderer;
        Character *pCharacter;
    }Game;

int initializations(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void handleInput(Game *pGame, SDL_Event *pEvent);

int main (int argument, char* arguments[]){
    Game game={0};
    if(!initializations(&game)) 
        return 1;
    run(&game);
    close(&game);
    
    return 0;
}

int initializations(Game *pGame){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0){
        printf("Error: %s\n",SDL_GetError());
        return 0;
    }

    pGame->pWindow = SDL_CreateWindow("Snomos",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1200,800,0);
    if(!pGame->pWindow){
        printf("Error: %s\n",SDL_GetError());
        close(pGame);
        return 0;
    }

    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!pGame->pRenderer){
        printf("Error: %s\n",SDL_GetError());
        close(pGame);
        return 0;    
    }

    pGame->pCharacter = createCharacter(600,400,pGame->pRenderer,1200,800);

    if(!pGame->pCharacter){
        printf("Error: %s\n",SDL_GetError());
        close(pGame);
        return 0;
    }

    return 1;
}

void run(Game *pGame){
    bool active = true;
    SDL_Event event;

    while(active){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT) 
                active = false;
            else handleInput(pGame,&event);
        }
        SDL_SetRenderDrawColor(pGame->pRenderer,0,0,0,255);
        SDL_RenderClear(pGame->pRenderer);
        SDL_SetRenderDrawColor(pGame->pRenderer,230,230,230,255);
        updateCharacter(pGame->pCharacter);
        drawCharacter(pGame->pCharacter);
        SDL_RenderPresent(pGame->pRenderer);
        SDL_Delay(1000/60-15);
    }
}

void close(Game *pGame){
    if(pGame->pCharacter){
        destroyCharacter(pGame->pCharacter);
    }
    if(pGame->pRenderer){ 
        SDL_DestroyRenderer(pGame->pRenderer);
    }
    if(pGame->pWindow){ 
        SDL_DestroyWindow(pGame->pWindow);
    }
    SDL_Quit();
}

void handleInput(Game *pGame, SDL_Event *pEvent){
    switch(pEvent->type){
        case SDL_KEYDOWN:
            switch(pEvent->key.keysym.scancode){
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    characterTurnUp(pGame->pCharacter);
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    characterTurnLeft(pGame->pCharacter);
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    characterTurnDown(pGame->pCharacter);
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    characterTurnRight(pGame->pCharacter);
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(pEvent->key.keysym.scancode){
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    characterYStop(pGame->pCharacter);
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    characterXStop(pGame->pCharacter);
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    characterYStop(pGame->pCharacter);
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    characterXStop(pGame->pCharacter);
                    break;  
            }                                
    }
}