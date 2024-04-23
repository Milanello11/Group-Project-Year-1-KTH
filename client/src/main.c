#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_net.h>
#include <stdbool.h>
#include <stdio.h>
#include "character.h"
#include "snowball.h"
#include "characterData.h"
typedef struct{
        SDL_Window *pWindow;
        SDL_Renderer *pRenderer;
        Character *pCharacter[CHARACTERS];
        //Character *pTmpChar;
        int nrOfCharacters, characterNumber;
        GameState state;
        UDPpacket *pPacket;
        UDPsocket pSocket;
        IPaddress serverAddress;
        ServerData sData;
        
        Snowball *pSnowball;
    }Game;

int initializations(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void handleInput(Game *pGame, SDL_Event *pEvent , bool *pSnowball);
void updateWithServerData(Game *pGame);

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
    
    if(SDLNet_ResolveHost(&(pGame->serverAddress), "127.0.0.1", 2000)){
        printf("SDLNet_ResolveHost(127.0.0.1 2000): %s\n", SDLNet_GetError());
        return 0;
    }
    
    if (!(pGame->pPacket = SDLNet_AllocPacket(512)))
	{
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		close(pGame);
        return 0;
	}


    pGame->pCharacter = createCharacter(600,400,pGame->pRenderer,1200,800);
    //pGame->pTmpChar = createCharacter(400,400,pGame->pRenderer,1200,800);
    
    if(!pGame->pPacket){
        printf("Error: %s\n",SDL_GetError());
        close(pGame);
        return 0;
    }

    for(int i=0;i<MAX_PLAYERS;i++)
        pGame->pCharacter[i] = createCharacter(i,pGame->pRenderer,WINDOW_WIDTH,WINDOW_HEIGHT);
    pGame->nrOfCharacters = MAX_PLAYERS;
    //pGame->pOverText = createText(pGame->pRenderer,238,168,65,pGame->pFont,"Game over",WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    //pGame->pStartText = createText(pGame->pRenderer,238,168,65,pGame->pScoreFont,"Waiting for clients",WINDOW_WIDTH/2,WINDOW_HEIGHT/2+100);
    for(int i=0;i<MAX_PLAYERS;i++){
        if(!pGame->pCharacter[i]){
            printf("Error: %s\n",SDL_GetError());
            close(pGame);
            return 0;
        }
    }
    pGame->state = START;
    return 1;
}

void run(Game *pGame){
    bool active = true;
    bool snowball = false;
    SDL_Event event;
    ClientData cData;
    int joining = 0;
    while(active){
        switch (pGame->state){

            case ONGOING:
                while (SDLNet_UDP_recv(pGame->pSocket, pGame->pPacket)){
                    updateWithServerData(pGame);
                }
                while(SDL_PollEvent(&event)){
                    if(event.type==SDL_QUIT) 
                        active = false;
                    else handleInput(pGame,&event,&snowball);
                }
                for(int i=0;i<MAX_PLAYERS;i++)
                    updateCharacter(pGame->pCharacter[i]);
                SDL_SetRenderDrawColor(pGame->pRenderer,0,0,0,255);
                SDL_RenderClear(pGame->pRenderer);
                SDL_SetRenderDrawColor(pGame->pRenderer,230,230,230,255);
                drawCharacter(pGame->pCharacter);
                if (snowball){
                    pGame->pSnowball = createSnowball(300,300,0,0,pGame->pRenderer,1200,800);
                    updateSnowball(pGame->pSnowball);
                    drawSnowball(pGame->pSnowball);
                }
                SDL_RenderPresent(pGame->pRenderer);
                
                break;
            
            case GAME_OVER:
                
            case START:
                if(!joining){
                    
                }else{
                SDL_SetRenderDrawColor(pGame->pRenderer,0,0,0,255);
                SDL_RenderClear(pGame->pRenderer);
                SDL_SetRenderDrawColor(pGame->pRenderer,230,230,230,255);
                
                if(joining) SDLNet_UDP_Send(pGame->pSocket, -1, pGame->pPacket);
                if(SDLNet_UDP_Recv(pGame->pSocket,pGame->pPacket)==1){
                    updateWithServerData(pGame);
                    //add(pGame->pPacket->address,pGame->clients,&(pGame->nrOfClients));
                    if(pGame->nrOfCharacters==CHARACTERS) setUpGame(pGame);
                }
                break;
            }       
        SDL_Delay(1000/60-15);
        }
    }
}

void setUpGame(Game *pGame){
    //for (int i=0;i<MAX_PLAYERS;i++) resetCharacter(pGame->pCharacter[i]);
    pGame->nrOfCharacters=MAX_PLAYERS;
    pGame->state = ONGOING; 
}

void updateWithServerData(Game *pGame){
    pGame->sData.gState = pGame->state;
    for(int i=0;i<MAX_PLAYERS;i++){
        getCharacterSendData(pGame->pCharacter[i], &(pGame->sData.rockets[i]));
    }
    for(int i=0;i<MAX_PLAYERS;i++){
        pGame->sData.playerNr = i;
        memcpy(pGame->pPacket->data, &(pGame->sData), sizeof(ServerData));
		pGame->pPacket->len = sizeof(ServerData);
        pGame->pPacket->address = pGame->character[i];
		SDLNet_UDP_Send(pGame->pSocket,-1,pGame->pPacket);
    }
}  

void add(IPaddress address, IPaddress clients[],int *pNrOfClients){
	for(int i=0;i<*pNrOfClients;i++) if(address.host==clients[i].host &&address.port==clients[i].port) return;
	clients[*pNrOfClients] = address;
	(*pNrOfClients)++;
}

void close(Game *pGame){
    if(pGame->pCharacter){
        destroyCharacter(pGame->pCharacter);
    }
    if(pGame->pSnowball){
        destroySnowball(pGame->pSnowball);
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