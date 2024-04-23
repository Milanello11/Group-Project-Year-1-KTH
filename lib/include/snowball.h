#ifndef snowball_h
#define snowball_h
#include "character.h"

typedef struct snowball Snowball;

Snowball *createSnowball(SDL_Renderer *pRenderer, int window_width , int window_height, Character *pCharacter);
void updateSnowball(Snowball *pSnowball);
void drawSnowball(Snowball *pSnowball);
void destroySnowball(Snowball *pSnowball);

#endif