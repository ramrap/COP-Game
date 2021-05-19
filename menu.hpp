#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <unistd.h>
#include <iostream>
#include "font.hpp"
#include "constans.hpp"


void winningscreen(int winner,SDL_Renderer *renderer,TTF_Font *font);
void server_or_client(SDL_Renderer *renderer, char *menu, TTF_Font *font);
void ask_for_ip(SDL_Renderer *renderer, TTF_Font *font, char *ip);

#endif
