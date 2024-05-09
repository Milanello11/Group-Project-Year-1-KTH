#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <stdbool.h>
#include "character.h"
#include "snowball.h"
#include "sounds.h"

void collisionManagement(Character *pCharacter, Snowball *pSnowball[], int characterNumber, Sounds *pSounds, UDPsocket socket, UDPpacket *pPacket){
    SDL_Rect snowballRect, characterRect;
    ClientData cData;
    if(checkCharacterAlive(pCharacter)){
    characterRect = getCharacterRect(pCharacter);
        for (int i = 0; i < MAXSNOWBALLS; i++){
            //printf("Snowball: %d\n",i );
            snowballRect = getSnowballRect(pSnowball[i]);
            if(!isColliding(characterRect, snowballRect)){
                resetSnowball(pSnowball[i]);
                setCharacterDead(pCharacter);
                playHitEffect(pSounds);
                cData.command = DEAD;
                cData.playerNumber = characterNumber;
                memcpy(pPacket->data, &cData, sizeof(ClientData));
                pPacket->len = sizeof(ClientData);
                SDLNet_UDP_Send(socket, -1,pPacket);
                printf("COLLISION\n");
                break;
            }
        }
    }
}