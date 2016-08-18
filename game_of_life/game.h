#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <stdio.h>
#include <time.h>

#define WHITE 0xFFFFFFFF
#define GREEN 0x22FF88FF
#define BLACK 0x000000FF
#define GRAY 0xCDCDCDFF

typedef enum State{IDLE, EDIT, PLAY, STEP, LOAD, QUIT}State;
typedef enum Mouse{NOPE, RMOUSE, LMOUSE, RMOTION, LMOTION}Mouse;

typedef struct Game{
    SDL_Event ev;
    SDL_TimerID id;
    SDL_Surface *screen;

    unsigned char **table;
    unsigned char* image;
    unsigned char scale;
    unsigned char treshold;
    unsigned char invert;
    unsigned short delay;
    unsigned short win_width;
    unsigned short win_height;
    unsigned char fname[150];
    unsigned short image_width;
    unsigned short image_height;
    unsigned short screen_width;
    unsigned short screen_height;
}Game;

Uint32 push_ev(Uint32 ms, void *param);
void init(Game *game);
void set_image(Game* game);
void destruct(Game *game);
int comp(int a, int b, unsigned char invert);
void clear_table(Game *game);
void draw_table(Game const *game);
void make_step(Game *game, short y, short x);
void step(Game *game);

#endif // GAME_H_
