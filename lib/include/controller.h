#ifndef controller_H
#define controller_H

void handleInput(Character *pCharacter, Snowball *pSnowball[], int characterNumber, SDL_Event *event, Sounds *pSounds, UDPsocket socket, UDPpacket *pPacket);

#endif