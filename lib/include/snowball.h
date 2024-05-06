#ifndef snowball_h
#define snowball_h
#define SNOWBALLSPEED 10
#define MAXSNOWBALLS 5
#include "characterData.h"


typedef struct snowball Snowball;

Snowball *createSnowball(SDL_Renderer *pRenderer, int window_width , int window_height);
void updateSnowball(Snowball *pSnowball);
void drawSnowball(Snowball *pSnowball);
void destroySnowball(Snowball *pSnowball);
void startSnowball(Snowball *pSnowball, int startX, int startY, int directionIndex);
int getOnScreenIndex(Snowball *pSnowball);
void getSnowballSendData(Snowball *pSnowball, SnowballData *pSnowballData);
void updateSnowballWithRecievedData(Snowball *pSnowball, SnowballData *pSnowballData);
void setSnowballOwner(Snowball *pSnowball, int number);
int getSnowballOwner(Snowball *pSnowball);
SDL_Rect getSnowballRect(Snowball *pSnowball);
void resetSnowball(Snowball *pSnowball);

#endif