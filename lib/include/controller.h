#ifndef controller_h
#define controller_h

void handleInput(Character *pCharacter, Snowball *pSnowball[], int characterNumber, SDL_Event *event, Sounds *pSounds, UDPsocket socket, UDPpacket *pPacket);

#endif