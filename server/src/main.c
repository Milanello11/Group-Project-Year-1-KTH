#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include "character.h"
#include "snowball.h"
#include "characterData.h"
#include "text.h"

typedef struct{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Character *pCharacter[CHARACTERS];
    GameState state;
    UDPpacket *pPacket;
    UDPsocket pSocket;
    IPaddress clients[CHARACTERS];
    int nrOfClients;
    ServerData sData;
    Snowball *pSnowball[MAXSNOWBALLS];
    TTF_Font *pScoreFont;
    Text *pStartText, *pEndText;
}Game;

int initializations(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void add(IPaddress address, IPaddress clients[], int *pNrOfClients);
void sendGameData(Game *pGame);
void executeCommand(Game *pGame, ClientData cData, int *pNrOfCharacters);
void changeState(Game *pGame);

int main (int argument, char* arguments[]){
    Game game={0};
    if(!initializations(&game)){ 
        return 1;
    }
    run(&game);
    close(&game);
    
    return 0;
}

int initializations(Game *pGame){
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        printf("Error: %s\n",SDL_GetError());
        return 0;
    }
    if(TTF_Init()!=0){
        printf("Error: %s\n",TTF_GetError());
        SDL_Quit();
        return 0;
    }
    if(SDLNet_Init()!=0){
        printf("Error: %s\n", SDLNet_GetError());
        TTF_Quit();
        SDL_Quit();
	    return 0;
	}

    pGame->pWindow = SDL_CreateWindow("Snomos Server",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,0);
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

    pGame->pScoreFont = TTF_OpenFont("../lib/resources/arial.ttf", 30);
    if(!pGame->pScoreFont){
        printf("Error: %s\n",TTF_GetError());
        close(pGame);
        return 0;
    }

    if(!(pGame->pSocket = SDLNet_UDP_Open(2069))){
		printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		close(pGame);
        return 0;
	}
    
	if(!(pGame->pPacket = SDLNet_AllocPacket(512))){
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		close(pGame);
        return 0;
	}

    for (int i = 0 ; i < CHARACTERS ; i++){
        pGame->pCharacter[i] = createCharacter(i,pGame->pRenderer,WINDOW_WIDTH,WINDOW_HEIGHT);
    }

    for(int i = 0 ; i < CHARACTERS ; i++){
        if(!pGame->pCharacter[i]){
            printf("Error: %s\n" , SDL_GetError());
            close(pGame);
            return 0;
        }
    }

    for(int i = 0; i < MAXSNOWBALLS; i++){
        pGame->pSnowball[i] = createSnowball(pGame->pRenderer , WINDOW_WIDTH , WINDOW_HEIGHT);
    }
    for(int i = 0 ; i < MAXSNOWBALLS ; i++){
        if(!pGame->pSnowball[i]){
            printf("Error: %s\n" , SDL_GetError());
            close(pGame);
        }
    }

    pGame->pStartText = createText(pGame->pRenderer,238,168,65,pGame->pScoreFont,"Waiting for clients",WINDOW_WIDTH/2,WINDOW_HEIGHT/2+100);
    pGame->pEndText = createText(pGame->pRenderer,238,168,65,pGame->pScoreFont,"Press 'R' to restart",WINDOW_WIDTH/2,WINDOW_HEIGHT/2+100);
    pGame->state = START;
    pGame->nrOfClients = 0;

    return 1;
}

void run(Game *pGame){

    bool active = true;
    SDL_Event event;
    ClientData cData;

    while(active){
        switch (pGame->state){
            case START:
                drawText(pGame->pStartText);
                SDL_RenderPresent(pGame->pRenderer);
                if(SDL_PollEvent(&event) && event.type==SDL_QUIT){ 
                    active = false;
                }
                if(SDLNet_UDP_Recv(pGame->pSocket,pGame->pPacket)==1){
                    add(pGame->pPacket->address,pGame->clients,&(pGame->nrOfClients));
                    printf("Number of Clients: %d / %d\n", pGame->nrOfClients, CHARACTERS);
                    if(pGame->nrOfClients == CHARACTERS){ 
                        changeState(pGame);
                        sendGameData(pGame);
                    }
                }
                break;
            case ONGOING: 
                while(SDLNet_UDP_Recv(pGame->pSocket,pGame->pPacket)==1){
                    memcpy(&cData, pGame->pPacket->data, sizeof(ClientData));
                    executeCommand(pGame,cData, &(pGame->nrOfClients));
                }
                if(SDL_PollEvent(&event)){
                    if(event.type==SDL_QUIT){ 
                        active = false;
                    }
                }
                for (int i = 0; i < CHARACTERS; i++){
                    updateCharacter(pGame->pCharacter[i]);    
                }
                changeState(pGame);
                sendGameData(pGame);
                SDL_SetRenderDrawColor(pGame->pRenderer,0,0,0,255);
                SDL_RenderClear(pGame->pRenderer);
                SDL_RenderPresent(pGame->pRenderer);
                SDL_Delay(1000/60);
                break;
            case GAME_OVER:
                sendGameData(pGame);
                drawText(pGame->pEndText);
                SDL_RenderPresent(pGame->pRenderer);
                if(SDL_PollEvent(&event)){
                    if(event.type == SDL_QUIT){
                        active = false;
                    }
                    else if(event.key.keysym.scancode == SDL_SCANCODE_R){
                        for(int i = 0; i < CHARACTERS; i++){
                            resetCharacter(pGame->pCharacter[i], i);
                        }
                        changeState(pGame);
                        sendGameData(pGame);
                    }
                }
                break;
        }       
    }
}

void changeState(Game *pGame){
    if(pGame->state == START){
        pGame->state = ONGOING;
    }
    else if(pGame->state == ONGOING && pGame->nrOfClients <= 1){
        pGame->state = GAME_OVER;
        printf("Game is over\n");
    }
    else if(pGame->state == GAME_OVER){
        pGame->state = ONGOING;
        pGame->nrOfClients = CHARACTERS;
    }
}

void sendGameData(Game *pGame){
    pGame->sData.gState = pGame->state;
    for(int i=0;i<CHARACTERS;i++){
        getCharacterSendData(pGame->pCharacter[i], &(pGame->sData.characters[i]));
    }
    for(int i = 0; i < MAXSNOWBALLS; i++){
        updateSnowball(pGame->pSnowball[i]);
        getSnowballSendData(pGame->pSnowball[i], &(pGame->sData.snowballData[i]));
    }
    for(int i=0;i<CHARACTERS;i++){
        pGame->sData.playerNumber = i;
        memcpy(pGame->pPacket->data, &(pGame->sData), sizeof(ServerData));
		pGame->pPacket->len = sizeof(ServerData);
        pGame->pPacket->address = pGame->clients[i];
		SDLNet_UDP_Send(pGame->pSocket,-1,pGame->pPacket);
    }
}  

void add(IPaddress address, IPaddress clients[],int *pNrOfClients){
	for(int i=0;i<*pNrOfClients;i++){
        if(address.host==clients[i].host && address.port==clients[i].port){
            return; 
        }
    }
    clients[*pNrOfClients] = address;
    (*pNrOfClients)++;
}

void executeCommand(Game *pGame, ClientData cData, int *pNrOfCharacters){
    switch (cData.command){
        case UP:
            characterTurnUp(pGame->pCharacter[cData.playerNumber]);
            break;
        case DOWN:
            characterTurnDown(pGame->pCharacter[cData.playerNumber]);
            break;
        case LEFT:
            characterTurnLeft(pGame->pCharacter[cData.playerNumber]);
            break;
        case RIGHT:
            characterTurnRight(pGame->pCharacter[cData.playerNumber]);
            break;
        case SHOOT:
            int check = -1;
            int owner = -1;

            for (int i = 0; i < MAXSNOWBALLS; i++){
                owner = getSnowballOwner(pGame->pSnowball[i]);
                if (owner == cData.playerNumber){
                    setActiveSnowballTrue(pGame->pCharacter[cData.playerNumber]);
                    break;
                }
                setActiveSnowballFalse(pGame->pCharacter[cData.playerNumber]);
            }
            
            if(getActiveSnowball(pGame->pCharacter[cData.playerNumber])){
                check = 1;
            }

            if(check == -1){
                int ssx = getPlayerXPos(pGame->pCharacter[cData.playerNumber]);
                int ssy = getPlayerYPos(pGame->pCharacter[cData.playerNumber]);
                int found = -1;

                for(int i = 0; i < MAXSNOWBALLS;i++){
                    if(!getOnScreenIndex(pGame->pSnowball[i])){
                        found = i;
                        break;
                    }
                }
                if(found >= 0){
                    int direction = getPlayerDirection(pGame->pCharacter[cData.playerNumber]);
                    startSnowball(pGame->pSnowball[found], ssx, ssy, direction);
                    setActiveSnowballTrue(pGame->pCharacter[cData.playerNumber]);
                    setSnowballOwner(pGame->pSnowball[found], cData.playerNumber);
                }
            }
            break;
        case STOPX:
            characterXStop(pGame->pCharacter[cData.playerNumber]);
            break;
        case STOPY:
            characterYStop(pGame->pCharacter[cData.playerNumber]);
            break;
        case DEAD:
            characterXStop(pGame->pCharacter[cData.playerNumber]);
            characterYStop(pGame->pCharacter[cData.playerNumber]);
            setCharacterDead(pGame->pCharacter[cData.playerNumber]);
            (*pNrOfCharacters)--;
            break;
    }
}

void close(Game *pGame){
    for(int i=0; i < CHARACTERS;i++){
        if(pGame->pCharacter[i]){
            destroyCharacter(pGame->pCharacter[i]);
        }
    }
    for(int i=0; i < MAXSNOWBALLS;i++){
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
    if(pGame->pStartText){
        destroyText(pGame->pStartText);  
    }
    if(pGame->pEndText){
        destroyText(pGame->pEndText);
    }
    if(pGame->pScoreFont){
        TTF_CloseFont(pGame->pScoreFont);
    }
    if(pGame->pPacket){ 
        SDLNet_FreePacket(pGame->pPacket);
    }
	if(pGame->pSocket){ 
        SDLNet_UDP_Close(pGame->pSocket);
    }

    TTF_Quit();
    SDLNet_Quit();
    SDL_Quit();
}