#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include "character.h"
#include "snowball.h"
#include "characterData.h"
#include "map.h"
#include "button.h"
#include "text.h"
#include "sounds.h"
#include "controller.h"
#include "logic.h"
#include "view.h"

typedef struct{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Character *pCharacter[CHARACTERS];
    int characterNumber;
    GameState state;
    UDPpacket *pPacket;
    UDPsocket pSocket;
    IPaddress serverAddress;
    ServerData sData;
    Snowball *pSnowball[MAXSNOWBALLS];
    Background *pBackground;
    Background *pMenuBackground;
    Background *pCreditBackground;
    Background *pWinnerBackground;
    Background *pLoserBackground;
    Button *pButton[NROFBUTTONS];
    TTF_Font *pFont, *pScoreFont;
    Text *pStartText;
    Sounds *pSounds;
}Game;

int initializations(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void updateWithServerData(Game *pGame);

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
        printf("Error: %s\n", SDL_GetError());
        return 0;
    }
    if(TTF_Init() != 0){
        printf("Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 0;
    }
    if(SDLNet_Init() != 0){
		printf("Error: %s\n", SDLNet_GetError());
        TTF_Quit(); 
        SDL_Quit();
		return 0;
	}
    pGame->pWindow = SDL_CreateWindow("Snömos", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(!pGame->pWindow){
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!pGame->pRenderer){
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;    
    }
    pGame->pBackground = createBackground(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!pGame->pBackground){
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    initBackground();
    if (!(pGame->pSocket = SDLNet_UDP_Open(0))){
		printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        return 0;
	}
    if(SDLNet_ResolveHost(&(pGame->serverAddress), "127.0.0.1", 2069)){
        printf("SDLNet_ResolveHost(127.0.0.1 2069): %s\n", SDLNet_GetError());
        return 0;
    }
    if (!(pGame->pPacket = SDLNet_AllocPacket(512))){
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		close(pGame);
        return 0;
	}
    if(!pGame->pPacket){
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    pGame->pFont = TTF_OpenFont("../lib/resources/arial.ttf", 40);
    if(!pGame->pFont){
        printf("Error: %s\n", TTF_GetError());
        close(pGame);
        return 0;
    }
    if(SDL_Init(SDL_INIT_AUDIO) < 0){
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("Error: %s\n", Mix_GetError());
        close(pGame);
        return 0;
    }

    pGame->pStartText = createText(pGame->pRenderer, 0, 0, 0, pGame->pFont, "Waiting for server", WINDOW_WIDTH/2, WINDOW_HEIGHT/2+100);
    pGame->pPacket->address.host = pGame->serverAddress.host;
    pGame->pPacket->address.port = pGame->serverAddress.port;

    for(int i = 0; i < CHARACTERS; i++){
        pGame->pCharacter[i] = createCharacter(i, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    for(int i = 0; i < CHARACTERS; i++){
        if(!pGame->pCharacter[i]){
            printf("Error: %s\n", SDL_GetError());
            close(pGame);
            return 0;
        }
    }
    for (int i = 0; i < MAXSNOWBALLS; i++){
        pGame->pSnowball[i] = createSnowball(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    for(int i = 0; i < MAXSNOWBALLS; i++){
        if(!pGame->pSnowball[i]){
            printf("Error: %s\n", SDL_GetError());
            close(pGame);
            return 0;
        }
    }
    pGame->pMenuBackground = createMenuBackground(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!pGame->pMenuBackground){
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    pGame->pCreditBackground = createCreditBackground(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!pGame->pCreditBackground){
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0; 
    }
    pGame->pWinnerBackground = createWinnerBackground(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!pGame->pWinnerBackground){
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0; 
    }
    pGame->pLoserBackground = createLoserBackground(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!pGame->pLoserBackground){
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0; 
    }
    for (int i = 0; i < NROFBUTTONS; i++){
        pGame->pButton[i] = createButton(pGame->pRenderer, 0, 0, 400, 100);
        setDesRect(pGame->pButton[i], i);
    }

    pGame->pSounds = createSounds();
    pGame->state = MENU;
    playMenuMusic(pGame->pSounds);
    return 1;
}

void run(Game *pGame){

    bool active = true;
    int joining = 0;
    bool control = true;
    int mouseX, mouseY;
    SDL_Event event;
    SDL_Rect snowballRect, characterRect;

    while(active){
        switch (pGame->state){
            case MENU:
                if(SDL_PollEvent(&event)){
                    SDL_GetMouseState(&mouseX, &mouseY);
                    menuController(&event, mouseX, mouseY, pGame->pSounds, &active, &(pGame->state));
                }
                drawMenu(pGame->pRenderer, pGame->pMenuBackground, pGame->pButton, mouseX, mouseY);
                break;
            case CREDITS:
                if(SDL_PollEvent(&event)){
                    SDL_GetMouseState(&mouseX, &mouseY);
                    menuController(&event, mouseX, mouseY, pGame->pSounds, &active, &(pGame->state));
                }
                drawCredits(pGame->pRenderer, pGame->pCreditBackground, pGame->pButton, mouseX, mouseY);
                break;
            case JOIN:
                if(!joining){
                    drawJoin(pGame->pRenderer, pGame->pMenuBackground, pGame->pButton, mouseX, mouseY);
                }
                if(SDL_PollEvent(&event)){
                    if(!joining){
                        SDL_GetMouseState(&mouseX, &mouseY);
                        joinController(&event, mouseX, mouseY, pGame->pSounds, pGame->pPacket, &(pGame->state), &joining, &active);
                    }
                }
                if(joining){
                    renderMenuBackground(pGame->pMenuBackground);
                    drawText(pGame->pStartText);
                    SDL_RenderPresent(pGame->pRenderer);
                    SDLNet_UDP_Send(pGame->pSocket, -1, pGame->pPacket);
                    closeController(&event, &active);
                }
                if(SDLNet_UDP_Recv(pGame->pSocket,pGame->pPacket)==1){
                    updateWithServerData(pGame);
                    if(pGame->state == ONGOING){
                        playGameplayMusic(pGame->pSounds);
                        joining = 0;
                    }
                }
                break;
            case ONGOING:
                while(SDLNet_UDP_Recv(pGame->pSocket, pGame->pPacket)){
                    updateWithServerData(pGame);
                }
                while(SDL_PollEvent(&event)){
                    closeController(&event, &active);
                    handleInput(pGame->pCharacter[pGame->characterNumber], pGame->pSnowball, pGame->characterNumber, &event, pGame->pSounds, pGame->pSocket, pGame->pPacket);
                }
                collisionManagement(pGame->pCharacter[pGame->characterNumber], pGame->pSnowball, pGame->characterNumber, pGame->pSounds, pGame->pSocket, pGame->pPacket);
                renderBackground(pGame->pRenderer, pGame->pBackground);
                for(int i = 0; i < CHARACTERS; i++){
                    updateCharacter(pGame->pCharacter[i]);
                    drawCharacter(pGame->pCharacter[i]);
                }
                for(int i = 0; i < MAXSNOWBALLS; i++){
                    updateSnowball(pGame->pSnowball[i]);
                    drawSnowball(pGame->pSnowball[i]);
                }
                SDL_RenderPresent(pGame->pRenderer);
                SDL_Delay(1000/60);
                break;
            case GAME_OVER:
                if(SDL_PollEvent(&event)){
                    closeController(&event, &active);
                }
                if(checkCharacterAlive(pGame->pCharacter[pGame->characterNumber]) && control){
                    control = false;
                    Mix_HaltMusic();
                    playWinSound(pGame->pSounds);
                    renderMenuBackground(pGame->pWinnerBackground);
                    SDL_RenderPresent(pGame->pRenderer);
                }
                else if(!(checkCharacterAlive(pGame->pCharacter[pGame->characterNumber])) && control){
                    control = false;
                    playLoseMusic(pGame->pSounds);
                    renderMenuBackground(pGame->pLoserBackground);
                    SDL_RenderPresent(pGame->pRenderer);
                }
                if(SDLNet_UDP_Recv(pGame->pSocket, pGame->pPacket) == 1){
                    control = true;
                    Mix_HaltMusic();
                    playGameplayMusic(pGame->pSounds);
                    for(int i = 0; i < MAXSNOWBALLS; i++){
                        resetSnowball(pGame->pSnowball[i]);
                    }
                    updateWithServerData(pGame);
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
    for(int i = 0; i < CHARACTERS; i++){
        updateCharacterWithRecievedData(pGame->pCharacter[i], &(sData.characters[i]));
    }
    for(int i = 0; i < MAXSNOWBALLS;i++){
        updateSnowballWithRecievedData(pGame->pSnowball[i], &(sData.snowballData[i]));
    }
}

void close(Game *pGame){
    for(int i = 0; i < CHARACTERS; i++){
        if(pGame->pCharacter){
            destroyCharacter(pGame->pCharacter[i]);
        }
    }
    for(int i = 0; i < MAXSNOWBALLS;i++){
        if(pGame->pSnowball[i]){
            destroySnowball(pGame->pSnowball[i]);
        }
    }
    for(int i = 0; i < NROFBUTTONS;i++){
        if(pGame->pButton[i]){
            destroyButton(pGame->pButton[i]);
        }
    }
    if(pGame->pBackground){
        destroyBackground(pGame->pBackground);
    }
    if(pGame->pMenuBackground){
        destroyBackground(pGame->pMenuBackground);
    }
    if(pGame->pCreditBackground){
        destroyBackground(pGame->pCreditBackground);
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
    if(pGame->pSounds){
        musicCleanup(pGame->pSounds);
    }

    TTF_Quit();
    SDLNet_Quit();
    SDL_Quit();
}