#ifndef controller_h
#define controller_h

void handleInput(Character *pCharacter, Snowball *pSnowball[], int characterNumber, SDL_Event *event, Sounds *pSounds, UDPsocket socket, UDPpacket *pPacket);
void menuController(SDL_Event *pEvent, int mouseX, int mouseY, Sounds *pSounds, bool *pActive, GameState *pState);
void joinController(SDL_Event *pEvent, int mouseX, int mouseY, Sounds *pSounds, UDPpacket *pPacket, GameState *pState, int *pJoining);

#endif