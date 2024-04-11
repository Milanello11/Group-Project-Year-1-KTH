#ifndef character_h
#define character_h

typedef struct character Character;

Character *createCharacter(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height);
void updateCharacter(Character *pCharacter);
void drawCharacter(Character *pCharacter);
void destroyCharacter(Character *pCharacter);



#endif