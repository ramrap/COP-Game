#include "menu.hpp"

void server_or_client(SDL_Renderer *renderer, char *menu, TTF_Font *font)
{
    SDL_Event e;
    int pressed = false;

    while (!pressed)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_c)
                {
                    *menu = 'c';
                    pressed = true;
                }
                else if (e.key.keysym.sym == SDLK_s)
                {
                    *menu = 's';
                    pressed = true;
                }
                else if (e.key.keysym.sym == SDLK_h)
                {
                    *menu = 'h';
                    pressed = true;
                }
            }
        }
        usleep(200);
        SDL_Rect rect;
        rect.w = 20 * TILE_SIZE;
        rect.h = 15 * TILE_SIZE;
        rect.x = TILE_SIZE * 0;
        rect.y = TILE_SIZE * 0;
        SDL_Surface *bitmap = NULL;
        SDL_Texture *tex = NULL;
        bitmap = SDL_LoadBMP("resources/home1.bmp");
        tex = SDL_CreateTextureFromSurface(renderer, bitmap);

        SDL_RenderCopy(renderer, tex, NULL, &rect);
        // SDL_RenderClear(renderer);
        TTF_Font *font2;
        font2 = TTF_OpenFont("resources/m5x7.ttf", 40);
        disp_text(renderer, "[s]erver or [c]lient?", font2, 260, 200);
        // disp_text(renderer, "[h]elp?", font, 260, 150);
        SDL_RenderPresent(renderer);
    }
}

void winningscreen(int winner, SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_RenderClear(renderer);
    SDL_Event e;
    int pressed = false;
    // std::cout<<"INSIDE WINNING SCREEN \n";

    SDL_Rect rect;
    rect.w = 20 * TILE_SIZE;
    rect.h = 15 * TILE_SIZE;
    rect.x = TILE_SIZE * 0;
    rect.y = TILE_SIZE * 0;
    SDL_Surface *bitmap = NULL;
    SDL_Texture *tex = NULL;
    bitmap = SDL_LoadBMP("resources/end.bmp");
    tex = SDL_CreateTextureFromSurface(renderer, bitmap);
    SDL_RenderCopy(renderer, tex, NULL, &rect);
    TTF_Font *font2;
    font2 = TTF_OpenFont("resources/m5x7.ttf", 40);

    if (winner == -1)
    {
        disp_text(renderer, "Game Ended Abrubptly!!", font2, 260, 200);
    }
    else if (winner == 1)
    {
        disp_text(renderer, "Guest Won", font2, 260, 200);
    }
    else if (winner == 0)
    {
        disp_text(renderer, "Host Won", font2, 260, 200);
    }

    // std::cout<<"hello \n";
    SDL_RenderPresent(renderer);
}
// return;

void ask_for_ip(SDL_Renderer *renderer, TTF_Font *font, char *ip)
{
    memset(ip, ' ', 15);
    SDL_Event e;
    int position = 0;
    int ok = false;
    while (!ok)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                if ((e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) || e.key.keysym.sym == SDLK_PERIOD)
                {
                    if (position > 14)
                    {
                        position = 14;
                    }
                    ip[position] = e.key.keysym.sym;
                    position++;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    position--;
                    if (position < 0)
                    {
                        position = 0;
                    }
                    ip[position] = ' ';
                }
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    ip[position] = 0;
                    ok = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(renderer);
        disp_text(renderer, "ip address", font, 240, 200);
        disp_text(renderer, ip, font, 240, 230);
        SDL_RenderPresent(renderer);
    }
}
