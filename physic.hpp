#ifndef PHYSIC_H
#define PHYSIC_H

#include <SDL.h>
#include <vector>
#include "objects.hpp"
#include "list.hpp"
#include "maze.hpp"


void getMap();
void move_player(struct Player *player); 
SDL_Texture* get_map_texture(SDL_Renderer *renderer);
void move_bullets(struct node **bullets);
int check_if_player_dies(struct Player *player, struct node **bullets, int *killer);

#endif
