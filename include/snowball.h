#ifndef snowball_h
#define snowball_h

typedef struct snowballImage SnowballImage;
typedef struct snowball Snowball;

SnowballImage *createSnowballImage(SDL_Renderer *pRenderer);
Snowball *createSnowball(SnowballImage *pSnowballImage, int window_width , int window_height);

void destroySnowballImage(SnowballImage *pSnowballImage);
void destroySnowball(Snowball *pSnowball);

/*
void updateSnowball(Snowball *pSnowball);
void drawSnowball(Snowball *pSnowball);

*/

#endif