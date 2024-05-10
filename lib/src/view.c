#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include "character.h"
#include "snowball.h"
#include "map.h"
#include "button.h"

void drawColor(SDL_Renderer *pRenderer, int x, int y, int z){
    SDL_SetRenderDrawColor(pRenderer, x, y, z, 255);
    SDL_RenderClear(pRenderer);
}
void drawMenu(Background *pMenuBackground, Button *pButton[], int mouseX, int mouseY){
    renderMenuBackground(pMenuBackground); //Funktion med en line i sig?
    drawButton(pButton[0], 0, hover(mouseX, mouseY, pButton[0]));
    drawButton(pButton[1], 1, hover(mouseX, mouseY, pButton[1]));
    drawButton(pButton[2], 2, hover(mouseX, mouseY, pButton[2]));
    drawButton(pButton[6], 6, hover(mouseX, mouseY, pButton[6]));
    drawButton(pButton[7], 7, hover(mouseX, mouseY, pButton[7]));
}