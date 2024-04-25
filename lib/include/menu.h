#ifndef menu_h
#define menu_h

typedef struct menu Menu;

Menu *createMenuBackground(SDL_Renderer *pRenderer, int window_w, int window_h);
void renderMenuBackground(Menu *pMenuBackground);

#endif