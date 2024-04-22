#ifndef map_h
#define map_h


void renderBackground(SDL_Renderer *pGameRender, SDL_Texture *tile_texture, SDL_Rect tiles_type[], SDL_Rect tile_placement[50][50]);
int getTileValue(int x, int y);
int getCollision(int x, int y);

#endif