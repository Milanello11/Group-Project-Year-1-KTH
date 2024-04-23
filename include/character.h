#ifndef character_h
#define character_h
#define CHARACTERS 1
#include <stdbool.h>

typedef struct character Character;

Character *createCharacter(int number, SDL_Renderer *pRenderer, int window_w, int window_h);
void updateCharacter(Character *pCharacter);
void drawCharacter(Character *pCharacter);
void destroyCharacter(Character *pCharacter);
//Behöver fixas
void characterTurnUp(Character *pCharacter);
void characterTurnDown(Character *pCharacter);
void characterTurnRight(Character *pCharacter);
void characterTurnLeft(Character *pCharacter);
void characterXStop(Character *pCharacter);
void characterYStop(Character *pCharacter);
bool isColliding(SDL_Rect a, SDL_Rect b);


#endif