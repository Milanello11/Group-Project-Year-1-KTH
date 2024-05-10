#ifndef view_h
#define view_h

void drawColor(SDL_Renderer *pRenderer, int x, int y, int z);
void drawMenu(SDL_Renderer *pRenderer, Background *pMenuBackground, Button *pButton[], int mouseX, int mouseY);
void drawCredits(SDL_Renderer *pRenderer, Background *pCreditBackground, Button *pButton[], int mouseX, int mouseY);
void drawJoin(SDL_Renderer *pRenderer, Background *pMenuBackground, Button *pButton[], int mouseX, int mouseY);

#endif