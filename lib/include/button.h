#ifndef button_h
#define button_h

typedef struct button Button;

Button *initButton(SDL_Renderer *pRenderer, int x, int y, int w, int h); 
void drawButton(Button *pButton , int index);
void destroyButton(Button *pButton);
int setSrcRectX(int x , int y);

#endif