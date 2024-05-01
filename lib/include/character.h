#ifndef character_h
#define character_h
#include <stdbool.h>
#include "characterData.h"

typedef struct character Character;

Character *createCharacter(int number, SDL_Renderer *pRenderer, int window_w, int window_h);
void updateCharacter(Character *pCharacter);
void drawCharacter(Character *pCharacter);
void destroyCharacter(Character *pCharacter);
void characterTurnUp(Character *pCharacter);
void characterTurnDown(Character *pCharacter);
void characterTurnRight(Character *pCharacter);
void characterTurnLeft(Character *pCharacter);
void characterXStop(Character *pCharacter);
void characterYStop(Character *pCharacter);
void updateCharacterWithRecievedData(Character *pCharacter, CharacterData *pCharacterData);
void getCharacterSendData(Character *pCharacter, CharacterData *pCharacterData);
bool isColliding(SDL_Rect a, SDL_Rect b);
int getPlayerXPos(Character *pCharacter);
int getPlayerYPos(Character *pCharacter);
bool getActiveSnowball(Character *pCharacter);
int getPlayerDirection(Character *pCharacter);
void setActiveSnowballTrue(Character *pCharacter);
void setActiveSnowballFalse(Character *pCharacter);
SDL_Rect getCharacterRect(Character *pCharacter);
void setCharacterDead(Character *pCharacter);
bool checkCharacterAlive(Character *pCharacter);

#endif