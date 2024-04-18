//Definerar alla funktioner nätverk behöver
#ifndef character_data_h
#define character_data_h

#define MAX_PLAYERS 6

enum clientCommand{UP, DOWN, LEFT, RIGHT, SHOOT};
typedef enum clientCommand ClientCommand;

struct clientData{
    clientCommand command;
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
};
typedef struct characterData CharacterData;

struct serverData{
    characterData characters[MAX_PLAYERS];
    int playerNumber;
};
typedef struct serverData ServerData;

#endif