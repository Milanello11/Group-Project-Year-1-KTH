#ifndef snowball_h
#define snowball_h

typedef struct snowball Snowball;

Snowball *createSnowball(int x , int y, int x_vel , int y_vel , SDL_Renderer *pRenderer, int window_w, int window_h);
void updateSnowball(Snowball *pSnowball);
void drawSnowball(Snowball *pSnowball);
void destroySnowball(Snowball *pSnowball);


#endif