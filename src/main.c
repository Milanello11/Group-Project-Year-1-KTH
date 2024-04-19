#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>
#include "character.h"
#include "snowball.h"
#include "map.h"



typedef struct{
        SDL_Window *pWindow;
        SDL_Renderer *pRenderer;
        Character *pCharacter;
        Character *pTmpChar;

        Snowball *pSnowball;
    }Game;

int initializations(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void handleInput(Game *pGame, SDL_Event *pEvent , bool *pSnowball);

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
    pGame->pTmpChar = createCharacter(400,400,pGame->pRenderer,1200,800);
    pGame->pSnowball = createSnowball(300,300,0,0,pGame->pRenderer,1200,800);
    if(!pGame->pCharacter){
        printf("Error: %s\n",SDL_GetError());
        close(pGame);
        return 0;
    }


    return 1;
}

void run(Game *pGame){
    bool active = true;
    bool snowball = false;
    SDL_Event event;

    SDL_Surface* tile_Map = IMG_Load("resources/tiles.png");
    SDL_Texture* tile_texture = SDL_CreateTextureFromSurface(pGame->pRenderer, tile_Map);
    SDL_FreeSurface(tile_Map);

    SDL_Rect tile_placement[32][32];
    for(int x = 0; x < 32; x++){
        for(int y = 0; y < 32; y++){
            tile_placement[x][y].x = x*16;
            tile_placement[x][y].y = y*16;
            tile_placement[x][y].w = 16;
            tile_placement[x][y].h = 16;
        }
    }

    SDL_Rect tiles_type[3];
    for (int i = 0; i < 3; i++) {
        tiles_type[i].x = i*16;
        tiles_type[i].y = 0;
        tiles_type[i].w = 16;
        tiles_type[i].h = 16;
    }


    while(active){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT) 
                active = false;
            else handleInput(pGame,&event,&snowball);
        }
        SDL_SetRenderDrawColor(pGame->pRenderer,0,0,0,255);
        SDL_RenderClear(pGame->pRenderer);
        renderBackground(pGame->pRenderer, tile_texture, tiles_type, tile_placement);
        updateCharacter(pGame->pCharacter, pGame->pTmpChar);
        drawCharacter(pGame->pCharacter);
        updateCharacter(pGame->pTmpChar, pGame->pCharacter);
        drawCharacter(pGame->pTmpChar);
        if (snowball){
            updateSnowball(pGame->pSnowball);
            drawSnowball(pGame->pSnowball);
        }
        SDL_RenderPresent(pGame->pRenderer);
        SDL_Delay(1000/60-15);
    }
}

void close(Game *pGame){
    if(pGame->pCharacter){
        destroyCharacter(pGame->pCharacter);
    }
    if(pGame->pSnowball){
        destroySnowball(pGame->pSnowball);
    }
    if(pGame->pTmpChar){
        destroyCharacter(pGame->pTmpChar);
    }
    if(pGame->pRenderer){ 
        SDL_DestroyRenderer(pGame->pRenderer);
    }
    if(pGame->pWindow){ 
        SDL_DestroyWindow(pGame->pWindow);
    }
    SDL_Quit();
}

void handleInput(Game *pGame, SDL_Event *pEvent, bool *pSnowball){
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
                case SDL_SCANCODE_SPACE:
                    *pSnowball = true;
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

void renderBackground(SDL_Renderer *pGameRender, SDL_Texture *tile_texture, SDL_Rect tiles_type[], SDL_Rect tile_placement[32][32]){

    for(int x = 0; x < 32; x++){
        for(int y = 0; y < 32; y++){
            switch (getTileValue(x, y)){

                case 0:
                    SDL_RenderCopy(pGameRender, tile_texture, &tiles_type[0], &tile_placement[x][y]);
                    break;
                case 1:
                    SDL_RenderCopy(pGameRender, tile_texture, &tiles_type[1], &tile_placement[x][y]);
                    break;
                case 2:
                    SDL_RenderCopy(pGameRender, tile_texture, &tiles_type[2], &tile_placement[x][y]);
                    break;
            }
        }
    }
}