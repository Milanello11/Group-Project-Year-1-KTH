#ifndef character_h
#define character_h
#include <stdbool.H>

typedef struct character Character;

Character *createCharacter(int x, int y, SDL_Renderer *pRenderer, int window_w, int window_h);
void updateCharacter(Character *pCharacter, Character *pTmpChar);
void drawCharacter(Character *pCharacter);
void destroyCharacter(Character *pCharacter);
void characterTurnUp(Character *pCharacter);
void characterTurnDown(Character *pCharacter);
void characterTurnRight(Character *pCharacter);
void characterTurnLeft(Character *pCharacter);
void characterXStop(Character *pCharacter);
void characterYStop(Character *pCharacter);
bool isColliding(SDL_Rect a, SDL_Rect b);


#endif