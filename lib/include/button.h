#ifndef button_h
#define button_h
#define NROFBUTTONS 4

typedef struct button Button;

Button *createButton(SDL_Renderer *pRenderer, int x, int y, int w, int h); 
void drawButton(Button *pButton , int index);
void destroyButton(Button *pButton);
int hover(int x , int y, Button *pButton);
void setDesRect(Button *pButton, int index);

#endif