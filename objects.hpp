#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL.h>
#include "physic.hpp"
#include "constans.hpp"

struct Player {
    SDL_Rect position;
    SDL_Texture *texture;
    int left, right, up, down;
    int can_jump, y_speed;
    int face, shoot, reloading;
    int left_key, right_key, up_key, down_key, attack_key;
    int kills, deaths;
    int wins;
    int powerA,powerATime, powerB, powerBTime;
};

struct Bullet {
    SDL_Rect position;
    int face;
    int player_id;
};

struct Power {
    SDL_Rect position;
};

struct PowerUps{
    int count = 0;
    int powerX[MAX_POWER];
    int powerY[MAX_POWER];
};







void resolve_player_key_up(int key, struct Player* player);
void resolve_player_key_down(int key, struct Player* player);
void resolve_keyboard(SDL_Event e, struct Player* player);
void set_player_pos(struct Player* player, float x, float y);
struct Bullet init_bullet(int x, int y, int face);
#endif
