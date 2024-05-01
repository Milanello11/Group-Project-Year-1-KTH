#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include "character.h"
#include "snowball.h"
#include "characterData.h"
#include "map.h"
#include "menu.h"
#include "button.h"
#include "text.h"

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
        Background *pBackground;
        Menu *pMenuBackground;
        Button *pButton[3];
        TTF_Font *pFont, *pScoreFont;
        Text *pOverText, *pStartText;
    }Game;

int initializations(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void handleInput(Game *pGame, SDL_Event *pEvent);
void updateWithServerData(Game *pGame);
bool snowballHit(Character *pCharacter, Snowball *pSnowball);

int main (int argument, char* arguments[]){
    Game game={0};
    if(!initializations(&game)) 
        return 1;
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

    pGame->pBackground = createBackground(pGame->pRenderer,WINDOW_WIDTH,WINDOW_HEIGHT);
    if(!pGame->pBackground){
        printf("Error: %s\n",SDL_GetError());
        close(pGame);
        return 0;
    }
    initBackground();
    if (!(pGame->pSocket = SDLNet_UDP_Open(0))){
		printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        return 0;
	}
    
    if(SDLNet_ResolveHost(&(pGame->serverAddress), "130.229.175.75", 2069)){
        printf("SDLNet_ResolveHost(127.0.0.1 2000): %s\n", SDLNet_GetError());
        return 0;
    }
    
    if (!(pGame->pPacket = SDLNet_AllocPacket(512))){
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		close(pGame);
        return 0;
	}
    
    if(!pGame->pPacket){
        printf("Error: %s\n",SDL_GetError());
        close(pGame);
        return 0;
    }

    pGame->pFont = TTF_OpenFont("../lib/resources/arial.ttf", 40);
    if(!pGame->pFont){
        printf("Error: %s\n",TTF_GetError());
        close(pGame);
        return 0;
    }

    pGame->pStartText = createText(pGame->pRenderer,0,105,255,pGame->pFont,"Waiting for server",WINDOW_WIDTH/2,WINDOW_HEIGHT/2+100);
    pGame->pOverText = createText(pGame->pRenderer,0,105,255,pGame->pFont,"Game over",WINDOW_WIDTH/2,WINDOW_HEIGHT/2+100);

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

    pGame->pMenuBackground = createMenuBackground(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!pGame->pMenuBackground){
        printf("Error: %s\n",SDL_GetError());
        close(pGame);
        return 0;
    }
    for (int i = 0; i < 3; i++){
        pGame->pButton[i] = createButton(pGame->pRenderer, 0, 0, 400, 100);
        setDesRect(pGame->pButton[i], i);
    }

    pGame->state = START;
    return 1;
}

void run(Game *pGame){

    int spritePos = 0;
    bool active = true;
    int joining = 0;

    SDL_Event event;
    ClientData cData;
    SDL_Texture* startButtonTexture = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Rect snowballRect, characterRect;

    while(active){
        switch (pGame->state){
            case ONGOING:
                while(SDLNet_UDP_Recv(pGame->pSocket, pGame->pPacket)){
                    updateWithServerData(pGame);
                }
                while(SDL_PollEvent(&event)){
                    if(event.type==SDL_QUIT) 
                        active = false;
                    else if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                        active = false;
                    }
                    else handleInput(pGame,&event);
                }
                characterRect = getCharacterRect(pGame->pCharacter[pGame->characterNumber]);
                for (int i = 0; i < MAXSNOWBALLS; i++){
                    snowballRect = getSnowballRect(pGame->pSnowball[i]);
                    if (isColliding(characterRect, snowballRect)){
                        printf("COLLISION\n");
                    }
                }

                SDL_SetRenderDrawColor(pGame->pRenderer,0,0,0,255);
                SDL_RenderClear(pGame->pRenderer);
                renderBackground(pGame->pRenderer, pGame->pBackground);
                SDL_SetRenderDrawColor(pGame->pRenderer,230,230,230,255);
                for(int i=0;i<CHARACTERS;i++){
                    updateCharacter(pGame->pCharacter[i]);
                    drawCharacter(pGame->pCharacter[i]);
                }
                for(int i=0;i<MAXSNOWBALLS;i++){
                    updateSnowball(pGame->pSnowball[i]);
                    drawSnowball(pGame->pSnowball[i]);
                }
                SDL_RenderPresent(pGame->pRenderer);
                SDL_Delay(1000/60);
                break;
            
            case GAME_OVER:
                /*updateWithServerData(pGame);
                SDL_RenderClear(pGame->pRenderer);
                drawText(pGame->pOverText);
                SDL_RenderPresent(pGame->pRenderer);
                if(SDL_PollEvent(&event)){
                    if(event.type==SDL_QUIT){ 
                        active = false;
                    }
                }*/
                break;
            case START:
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if(!joining){
                    SDL_SetRenderDrawColor(pGame->pRenderer,255,255,255,255);
                    SDL_RenderClear(pGame->pRenderer);
                    renderMenuBackground(pGame->pMenuBackground);
                    for(int i = 0; i < 3;i++){
                        if(hover(mouseX, mouseY, pGame->pButton[i])){
                            drawButton(pGame->pButton[i], i+3);
                        }
                        else{
                            drawButton(pGame->pButton[i], i);
                        }
                    }
                    SDL_RenderPresent(pGame->pRenderer);  
                }
                if(SDL_PollEvent(&event)){
                    if(event.type == SDL_QUIT){
                        active = false;
                    }
                    if(!joining && event.type == SDL_MOUSEBUTTONDOWN){
                        if(mouseX >= 200 && mouseX <= 600 && mouseY >= 200 && mouseY <= 355){
                            joining = 1;
                            cData.command = READY;
                            cData.playerNumber =- 1;
                            memcpy(pGame->pPacket->data, &cData, sizeof(ClientData));
                            pGame->pPacket->len = sizeof(ClientData);
                        }
                        if(mouseX >= 300 && mouseX <= 500 && mouseY >= 400 && mouseY <= 505){
                            active = false;
                        }
                    }
                    if(joining){
                        SDL_SetRenderDrawColor(pGame->pRenderer,255,255,255,255);
                        SDL_RenderClear(pGame->pRenderer);
                        drawText(pGame->pStartText);
                        SDL_RenderPresent(pGame->pRenderer);
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
    for(int i = 0; i < MAXSNOWBALLS;i++){
        updateSnowballWithRecievedData(pGame->pSnowball[i], &(sData.snowballData[i]));
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
    for(int i = 0; i < 3; i++){
        if(pGame->pButton[i]){
            destroyButton(pGame->pButton[i]);
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
    if(pGame->pStartText){
        destroyText(pGame->pStartText);  
    }
    if(pGame->pFont){
        TTF_CloseFont(pGame->pFont);
    } 

    TTF_Quit();
    SDLNet_Quit();
    SDL_Quit();
}

void handleInput(Game *pGame, SDL_Event *pEvent){
    ClientData cData;
    cData.playerNumber = pGame->characterNumber;
    switch(pEvent->type){
        case SDL_KEYDOWN:
            switch(pEvent->key.keysym.scancode){
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    characterTurnUp(pGame->pCharacter[pGame->characterNumber]);
                    cData.command = UP;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    characterTurnLeft(pGame->pCharacter[pGame->characterNumber]);
                    cData.command = LEFT;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    characterTurnDown(pGame->pCharacter[pGame->characterNumber]);
                    cData.command = DOWN;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    characterTurnRight(pGame->pCharacter[pGame->characterNumber]);
                    cData.command = RIGHT;
                    break;
                case SDL_SCANCODE_SPACE:
                    int check = -1;
                    int owner = -1;

                    for (int i = 0; i < MAXSNOWBALLS; i++){
                        owner = getSnowballOwner(pGame->pSnowball[i]);
                        if (owner == pGame->characterNumber){
                            setActiveSnowballTrue(pGame->pCharacter[pGame->characterNumber]);
                            break;
                        }
                        setActiveSnowballFalse(pGame->pCharacter[pGame->characterNumber]);
                    }
                    
                    if(getActiveSnowball(pGame->pCharacter[pGame->characterNumber])){
                        check = 1;
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
                            int direction = getPlayerDirection(pGame->pCharacter[pGame->characterNumber]);
                            startSnowball(pGame->pSnowball[found], ssx, ssy, direction);
                            setActiveSnowballTrue(pGame->pCharacter[pGame->characterNumber]);
                            setSnowballOwner(pGame->pSnowball[found], pGame->characterNumber);
                            cData.command = SHOOT;
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