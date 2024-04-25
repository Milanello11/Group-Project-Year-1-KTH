#ifndef map_h
#define map_h

typedef struct background Background;
Background *createBackground(SDL_Renderer *pRenderer, int window_w, int window_h);
void initBackground();
void renderBackground(SDL_Renderer *pGameRender, Background *pBackground);
int getTileValue(int x, int y);
int getCollision(int x, int y);

#endif