#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include "character.h"
#include "snowball.h"
#include "sounds.h"
#include "characterData.h"


void handleInput(Character *pCharacter, Snowball *pSnowball[], int characterNumber, SDL_Event *pEvent, Sounds *pSounds, UDPsocket socket, UDPpacket *pPacket){
    ClientData cData;
    int owner, found;
    int startSnowballX, startSnowballY, direction;
    if(checkCharacterAlive(pCharacter)){
        cData.playerNumber = characterNumber;
        switch(pEvent->type){
            case SDL_KEYDOWN:
                switch(pEvent->key.keysym.scancode){
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        characterTurnUp(pCharacter);
                        cData.command = UP;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        characterTurnLeft(pCharacter);
                        cData.command = LEFT;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        characterTurnDown(pCharacter);
                        cData.command = DOWN;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        characterTurnRight(pCharacter);
                        cData.command = RIGHT;
                        break;
                    case SDL_SCANCODE_SPACE:
                        owner = -1;
                        for (int i = 0; i < MAXSNOWBALLS; i++){
                            owner = getSnowballOwner(pSnowball[i]);
                            if (owner == characterNumber){
                                setActiveSnowballTrue(pCharacter);
                                break;
                            }
                            setActiveSnowballFalse(pCharacter);
                        }
                        if(!getActiveSnowball(pCharacter)){
                            startSnowballX = getPlayerXPos(pCharacter);
                            startSnowballY = getPlayerYPos(pCharacter);
                            found = -1;

                            for(int i = 0; i < MAXSNOWBALLS;i++){
                                if(!getOnScreenIndex(pSnowball[i])){
                                    found = i;
                                    break;
                                }
                            }
                            if(found >= 0){
                                direction = getPlayerDirection(pCharacter);
                                startSnowball(pSnowball[found], startSnowballX, startSnowballY, direction);
                                setActiveSnowballTrue(pCharacter);
                                setSnowballOwner(pSnowball[found], characterNumber);
                                playThrowEffect(pSounds);
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
                        characterYStop(pCharacter);
                        cData.command = STOPY;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        characterXStop(pCharacter);
                        cData.command = STOPX;
                        break;  
                }                                
        }
    }
    else{
        characterYStop(pCharacter);
        characterXStop(pCharacter);
    }
    memcpy(pPacket->data, &cData, sizeof(ClientData));
    pPacket->len = sizeof(ClientData);
    SDLNet_UDP_Send(socket, -1,pPacket);
}

void menuController(SDL_Event *pEvent, int mouseX, int mouseY, Sounds *pSounds, bool *pActive, GameState *pState){
    switch(*pState){
        case MENU:
            if(pEvent->type == SDL_QUIT || pEvent->key.keysym.scancode == SDL_SCANCODE_ESCAPE){
            *pActive = false;
        }
        if(pEvent->type == SDL_MOUSEBUTTONDOWN){
            if(mouseX >= 200 && mouseX <= 600 && mouseY >= 150 && mouseY <= 348){
                playButtonEffect(pSounds);
                *pState = JOIN;
            }
            if(mouseX >= 300 && mouseX <= 500 && mouseY >= 570 && mouseY <= 669){
                playButtonEffect(pSounds);
                SDL_Delay(100);
                *pActive = false;
            }
            if(mouseX >= 580 && mouseX <= 780 && mouseY >= 660 && mouseY <= 759){
                playButtonEffect(pSounds);
                *pState = CREDITS;
            }
        }
        break;
        case CREDITS:
            if(pEvent->type == SDL_QUIT || pEvent->key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                *pActive = false;
            }
            if(pEvent->type == SDL_MOUSEBUTTONDOWN){
                if(mouseX >= 300 && mouseX <= 500 && mouseY >= 660 && mouseY <= 765){
                    playButtonEffect(pSounds);
                    *pState = MENU;
                }
            }
    }

}




