#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_net.h>
#include <stdbool.h>
#include <stdio.h>
#include "character.h"
#include "snowball.h"
#include "characterData.h"
#include "map.h"

typedef struct{
        SDL_Window *pWindow;
        SDL_Renderer *pRenderer;
        Character *pCharacter[CHARACTERS];
        int nrOfCharacters, characterNumber;
        GameState state;
        UDPpacket *pPacket;
        UDPsocket pSocket;
        IPaddress serverAddress;
        ServerData sData;
        Snowball *pSnowball[MAXSNOWBALLS];
    }Game;

int initializations(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void handleInput(Game *pGame, SDL_Event *pEvent , int *pDirectionIndexl);
void updateWithServerData(Game *pGame);
void renderBackground(SDL_Renderer *pGameRender, SDL_Texture *tile_texture, SDL_Rect tiles_type[], SDL_Rect tile_placement[50][50]);


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


    pGame->pWindow = SDL_CreateWindow("Snomos",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,0);
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
    if(SDLNet_Init()){
		printf("Error: %s\n", SDLNet_GetError());
        close(pGame);
		return 0;
	}
    if (!(pGame->pSocket = SDLNet_UDP_Open(0)))
	{
		printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        return 0;
	}
    
    if(SDLNet_ResolveHost(&(pGame->serverAddress), "130.229.146.23", 2000)){
        printf("SDLNet_ResolveHost(127.0.0.1 2000): %s\n", SDLNet_GetError());
        return 0;
    }
    
    if (!(pGame->pPacket = SDLNet_AllocPacket(512)))
	{
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		close(pGame);
        return 0;
	}
    
    if(!pGame->pPacket){
        printf("Error: %s\n",SDL_GetError());
        close(pGame);
        return 0;
    }
    pGame->pPacket->address.host = pGame->serverAddress.host;
    pGame->pPacket->address.port = pGame->serverAddress.port;

    for(int i=0;i<CHARACTERS;i++){
        pGame->pCharacter[i] = createCharacter(i,pGame->pRenderer,WINDOW_WIDTH,WINDOW_HEIGHT);
    }
    pGame->nrOfCharacters = CHARACTERS;

    //pGame->pOverText = createText(pGame->pRenderer,238,168,65,pGame->pFont,"Game over",WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    //pGame->pStartText = createText(pGame->pRenderer,238,168,65,pGame->pScoreFont,"Waiting for clients",WINDOW_WIDTH/2,WINDOW_HEIGHT/2+100);

    for(int i=0;i<CHARACTERS;i++){
        if(!pGame->pCharacter[i]){
            printf("Error: %s\n",SDL_GetError());
            close(pGame);
            return 0;
        }
    }
    for (int i = 0; i < MAXSNOWBALLS; i++){
        pGame->pSnowball[i] = createSnowball(pGame->pRenderer , WINDOW_WIDTH , WINDOW_HEIGHT);
    }
    for(int i=0;i<MAXSNOWBALLS;i++){
        if(!pGame->pSnowball[i]){
            printf("Error: %s\n",SDL_GetError());
            close(pGame);
            return 0;
        }
    }
    pGame->state = ONGOING;
    return 1;
}

void run(Game *pGame){

    bool active = true;
    int directionIndex = 0;
    int joining = 0;

    SDL_Event event;
    ClientData cData;

    SDL_Surface* tile_Map = IMG_Load("../lib/resources/tiles.png");
    SDL_Texture* tile_texture = SDL_CreateTextureFromSurface(pGame->pRenderer, tile_Map);
    SDL_FreeSurface(tile_Map);

    SDL_Rect tile_placement[50][50];
    for(int x = 0; x < 50; x++){
        for(int y = 0; y < 50; y++){
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
        switch (pGame->state){
            case ONGOING:
                while (SDLNet_UDP_Recv(pGame->pSocket, pGame->pPacket)){
                    updateWithServerData(pGame);
                }
                while(SDL_PollEvent(&event)){
                    if(event.type==SDL_QUIT) 
                        active = false;
                    else handleInput(pGame,&event,&directionIndex);
                }
                SDL_SetRenderDrawColor(pGame->pRenderer,0,0,0,255);
                SDL_RenderClear(pGame->pRenderer);
                renderBackground(pGame->pRenderer, tile_texture, tiles_type, tile_placement);
                SDL_SetRenderDrawColor(pGame->pRenderer,230,230,230,255);
                for(int i=0;i<CHARACTERS;i++){
                    updateCharacter(pGame->pCharacter[i]);
                    drawCharacter(pGame->pCharacter[i]);
                }
                for(int i=0;i<MAXSNOWBALLS;i++){
                    if(getOnScreenIndex(pGame->pSnowball[i])){
                        updateSnowball(pGame->pSnowball[i]);
                        drawSnowball(pGame->pSnowball[i]);
                    }
                }
                    
                SDL_RenderPresent(pGame->pRenderer);
                SDL_Delay(1000/60-15);
                break;
            
            case GAME_OVER:
                
            case START:
                if(!joining){
                    
                }
                else{
                SDL_SetRenderDrawColor(pGame->pRenderer,0,0,0,255);
                SDL_RenderClear(pGame->pRenderer);
                SDL_SetRenderDrawColor(pGame->pRenderer,230,230,230,255);
                }
                SDL_RenderPresent(pGame->pRenderer);
                if(SDL_PollEvent(&event)){
                    if(event.type == SDL_QUIT){
                        active = false;
                    }
                    else if(!joining && event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_P){
                        joining = 1;
                        cData.command = READY;
                        cData.playerNumber =- 1;
                        memcpy(pGame->pPacket->data, &cData, sizeof(ClientData));
		                pGame->pPacket->len = sizeof(ClientData);
                    }
                }
                if(joining){
                SDLNet_UDP_Send(pGame->pSocket, -1, pGame->pPacket);
                }
                if(SDLNet_UDP_Recv(pGame->pSocket,pGame->pPacket)==1){
                    updateWithServerData(pGame);
                    if(pGame->state == ONGOING){
                        joining = 0;
                    }
                }
                break;
            }       
        SDL_Delay(1000/60-15);
        }
}


void updateWithServerData(Game *pGame){
    ServerData sData;
    memcpy(&sData, pGame->pPacket->data, sizeof(ServerData));
    pGame->characterNumber = sData.playerNumber;
    pGame->state = sData.gState;
    for(int i=0;i<CHARACTERS;i++){
        updateCharacterWithRecievedData(pGame->pCharacter[i], &(sData.characters[i]));
    } 
}

void close(Game *pGame){
    for(int i=0; i < CHARACTERS;i++){
        if(pGame->pCharacter){
            destroyCharacter(pGame->pCharacter[i]);
        }
    }
    for(int i = 0; i < MAXSNOWBALLS;i++){
        if(pGame->pSnowball[i]){
            destroySnowball(pGame->pSnowball[i]);
        }
    }

    if(pGame->pRenderer){ 
        SDL_DestroyRenderer(pGame->pRenderer);
    }
    if(pGame->pWindow){ 
        SDL_DestroyWindow(pGame->pWindow);
    }
    if(pGame->pPacket){ 
        SDLNet_FreePacket(pGame->pPacket);
    }
	if(pGame->pSocket){ 
        SDLNet_UDP_Close(pGame->pSocket);
    }

    SDLNet_Quit();
    SDL_Quit();
}

void handleInput(Game *pGame, SDL_Event *pEvent, int *pDirectionIndex){
    ClientData cData;
    cData.playerNumber = pGame->characterNumber;
    switch(pEvent->type){
        case SDL_KEYDOWN:
            switch(pEvent->key.keysym.scancode){
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    *pDirectionIndex = 0;
                    characterTurnUp(pGame->pCharacter[pGame->characterNumber]);
                    cData.command = UP;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    *pDirectionIndex = 3;
                    characterTurnLeft(pGame->pCharacter[pGame->characterNumber]);
                    cData.command = LEFT;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    *pDirectionIndex = 2;
                    characterTurnDown(pGame->pCharacter[pGame->characterNumber]);
                    cData.command = DOWN;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    *pDirectionIndex = 1;
                    characterTurnRight(pGame->pCharacter[pGame->characterNumber]);
                    cData.command = RIGHT;
                    break;
                case SDL_SCANCODE_SPACE:
                    int check = -1;
                    for(int i = 0; i < MAXSNOWBALLS;i++){
                        if(getOnScreenIndex(pGame->pSnowball[i]))
                        {
                            check = 1;
                            break;
                        }
                    }
                    if(check == -1){
                        int ssx = getPlayerXPos(pGame->pCharacter[pGame->characterNumber]);
                        int ssy = getPlayerYPos(pGame->pCharacter[pGame->characterNumber]);
                        int found = -1;

                        for(int i = 0; i < MAXSNOWBALLS;i++){
                            if(!getOnScreenIndex(pGame->pSnowball[i])){
                                found = i;
                                break;
                            }
                        }
                        if(found >= 0){
                            startSnowball(pGame->pSnowball[found], ssx, ssy, *pDirectionIndex);
                        }
                    }
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(pEvent->key.keysym.scancode){
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    characterYStop(pGame->pCharacter[pGame->characterNumber]);
                    cData.command = STOPY;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    characterXStop(pGame->pCharacter[pGame->characterNumber]);
                    cData.command = STOPX;
                    break;  
            }                                
    }
    memcpy(pGame->pPacket->data, &cData, sizeof(ClientData));
	pGame->pPacket->len = sizeof(ClientData);
    SDLNet_UDP_Send(pGame->pSocket, -1,pGame->pPacket);
}

void renderBackground(SDL_Renderer *pGameRender, SDL_Texture *tile_texture, SDL_Rect tiles_type[], SDL_Rect tile_placement[50][50]){

    for(int x = 0; x < 50; x++){
        for(int y = 0; y < 50; y++){
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