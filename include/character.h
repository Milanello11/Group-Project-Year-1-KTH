#ifndef character_h
#define character_h

typedef struct character Character;

Character *createCharacter(int x, int y, SDL_Renderer *pRenderer, int window_w, int window_h);
void updateCharacter(Character *pCharacter);
void drawCharacter(Character *pCharacter);
void destroyCharacter(Character *pCharacter);

#endif