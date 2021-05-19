#include "menu.hpp"


void server_or_client(SDL_Renderer *renderer, char *menu, TTF_Font *font){
    SDL_Event e;
    int pressed = false;
    disp_text(renderer, "COP290", font, 100, 100);
    while (!pressed) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_c) {
                    *menu = 'c';
                    pressed = true;
                } else if (e.key.keysym.sym == SDLK_s) {
                    *menu = 's';
                    pressed = true;
                }

            }
        }
        usleep(200);
        SDL_RenderClear(renderer);
        disp_text(renderer, "[s]erver or [c]lient?", font, 260, 200);
        disp_text(renderer, "[h]elp?", font, 260, 150);
        SDL_RenderPresent(renderer);
    }
}

void winningscreen(int winner,SDL_Renderer *renderer,TTF_Font *font){
    SDL_Event e;
    int pressed = false;
    std::cout<<"INSIDE WINNING SCREEN \n";
    while (!pressed) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_c) {
                    // *menu = 'c';
                    pressed = true;
                } else if (e.key.keysym.sym == SDLK_s) {
                    // *menu = 's';
                    pressed = true;
                }

            }
        }
        if(winner==-1){
            disp_text(renderer, "Game Ended Abrubptly!!", font, 260, 200);
        }
        else if(winner==1){
            disp_text(renderer, "Guest Won", font, 260, 200);
        }
        else if(winner==0){
            disp_text(renderer, "Host Won", font, 260, 200);
        }
        usleep(200);
        SDL_RenderClear(renderer);
        
        SDL_RenderPresent(renderer);
    }
    return;

}

void ask_for_ip(SDL_Renderer *renderer, TTF_Font *font, char *ip) {
    memset(ip, ' ', 15);
    SDL_Event e;
    int position = 0;
    int ok = false;
    while (!ok) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if ((e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) || e.key.keysym.sym == SDLK_PERIOD) {
                    if (position > 14) {
                        position = 14;
                    }
                    ip[position] = e.key.keysym.sym;
                    position++;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    position--;
                    if (position < 0) {
                        position = 0;
                    }
                    ip[position] = ' ';
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    ip[position] = 0;
                    ok = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(renderer);
        disp_text(renderer, "ip addres", font, 240, 200);
        disp_text(renderer, ip, font, 240, 230);
        SDL_RenderPresent(renderer);
    }
}
