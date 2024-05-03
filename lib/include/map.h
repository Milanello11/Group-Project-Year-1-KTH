#ifndef map_h
#define map_h

typedef struct background Background;
Background *createBackground(SDL_Renderer *pRenderer, int window_w, int window_h);
Background *createMenuBackground(SDL_Renderer *pRenderer, int window_w, int window_h);
Background *createCreditBackground(SDL_Renderer *pRenderer, int window_w, int window_h);
void initBackground();
void renderBackground(SDL_Renderer *pGameRender, Background *pBackground);
void renderMenuBackground(Background *pMenuBackground);
void renderCreditBackground(Background *pCreditBackground);
int getTileValue(int x, int y);
int getCollision(int x, int y);
void destroyBackground(Background *pBackground);

#endif