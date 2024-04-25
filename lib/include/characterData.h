#ifndef character_data_h
#define character_data_h
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800
#define CHARACTERS 2

enum gameState{START, ONGOING, GAME_OVER};
typedef enum gameState GameState;

enum clientCommand{READY, UP, DOWN, LEFT, RIGHT, SHOOT, STOPX, STOPY};
typedef enum clientCommand ClientCommand;

struct clientData{
    ClientCommand command;
    int playerNumber;
};
typedef struct clientData ClientData;

struct snowballData{
    float x_pos, y_pos, x_vel, y_vel;
    int window_width , window_height;
};
typedef struct snowballData SnowballData;

struct characterData{
    float x_pos, y_pos, x_vel, y_vel;
    int window_width , window_height;
    SnowballData ballData;
};
typedef struct characterData CharacterData;

struct serverData{
    CharacterData characters[CHARACTERS];
    int playerNumber;
    GameState gState;
};
typedef struct serverData ServerData;

#endif