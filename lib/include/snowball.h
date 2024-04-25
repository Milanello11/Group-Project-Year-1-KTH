#ifndef snowball_h
#define snowball_h
#define SNOWBALLSPEED 10
#define MAXSNOWBALLS 5


typedef struct snowball Snowball;

Snowball *createSnowball(SDL_Renderer *pRenderer, int window_width , int window_height);
void updateSnowball(Snowball *pSnowball);
void drawSnowball(Snowball *pSnowball);
void destroySnowball(Snowball *pSnowball);
void initSnowballToNull(Snowball **Snowball);
void startSnowball(Snowball *pSnowball, int startX, int startY, int directionIndex);
int getOnScreenIndex(Snowball *pSnowball);

void getSnowballSendData(Snowball *pSnowball, SnowballData *pSnowballData);
void updateSnowballWithRecievedData(Snowball *pSnowball, SnowballData *pSnowballData);





#endif