#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_mixer.h>

#include <set>
#include <utility>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include "objects.hpp"
#include "client_udp.hpp"
#include "server_udp.hpp"
#include "network.hpp"
#include "physic.hpp"
#include "constans.hpp"
#include "font.hpp"
#include "menu.hpp"

#include "audio.hpp"

struct Player players[MAX_PLAYERS];
int number_of_players = 0;
int16_t my_id = -1;
int16_t bullets_client[256];
int16_t winner =-1;

vector<pair<int,int>>power_array(MAX_POWER);

int bullets_number = 0;
bool running=true;

SDL_Texture *load_texture(SDL_Renderer *renderer, char *file)
{
    SDL_Surface *bitmap = NULL;
    SDL_Texture *texture = NULL;
    bitmap = SDL_LoadBMP(file);
    texture = SDL_CreateTextureFromSurface(renderer, bitmap);
    SDL_FreeSurface(bitmap);
    return texture;
}




void init_players()
{
    int i;
    for (i = 0; i < MAX_PLAYERS; i++)
    {

        players[i].position.x = SPAWN_X;
        players[i].position.y = SPAWN_Y;
        players[i].position.w = PLAYER_WIDTH;
        players[i].position.h = PLAYER_HEIGHT;
        players[i].left_key = SDLK_LEFT;
        players[i].right_key = SDLK_RIGHT;
        players[i].up_key = SDLK_UP;
        players[i].down_key = SDLK_DOWN;
        players[i].attack_key = SDLK_z;
        players[i].face = 1;
        players[i].shoot = false;
        players[i].y_speed = 0;
        players[i].can_jump = false;
        players[i].reloading = false;
        players[i].kills = 0;
        players[i].deaths = 0;
        players[i].powerA = 0;
        players[i].powerATime = 0;
        players[i].powerB = 0;
        players[i].powerBTime = 0;
        players[i].wins = 0;
    }
}

void receive_new_id(int id)
{
    my_id = id;
    number_of_players = id;
    printf("my_id is now: %d\n", my_id);
}

void check_if_its_new_player(int id)
{
    if (id > number_of_players)
    {
        number_of_players = id;
        printf("new max player, now %d\n", number_of_players + 1);
    }
}

void *client_loop(void *arg)
{
    int socket = *((int *)arg);
    int16_t tab[BUF_MAX];
    int length;
    int id, bullets_in_array;
    while (1)
    {
        length = client_listen(socket, tab);
        id = tab[0];
        if (id == -1)
        {
            receive_new_id(tab[1]);
        }
        if (id >= 0)
        {
            // cout<<"thread me he \n";
            check_if_its_new_player(id);
            players[id].position.x = tab[1];
            players[id].position.y = tab[2];
            players[id].kills = tab[3];
            players[id].deaths = tab[4];
            players[id].wins = tab[5];
            players[id].powerA = tab[6];
            // cout<<tab[5]<<" =<"<<id<<endl;
            if(players[id].wins>=1){
                
                winner=id;
                cout<<"Winner "<<id<<"\n";
            }
            
            for(int i=7;i<7+MAX_POWER;i++){
               
                power_array[i-7].first=tab[i];
                power_array[i-7].second=tab[i+MAX_POWER];
            }
            updatePowerArray(power_array);


        }
        if (id == -2)
        {
            bullets_in_array = (length - sizeof(int16_t)) / (sizeof(int16_t) * 2);
            memcpy(bullets_client, tab + 1, sizeof(int16_t) * 2 * bullets_in_array);
            bullets_number = bullets_in_array;
        }
        if(id == -3){
            running=false;
            winner=tab[1];
            cout<<"WINNER DECLARED \n";
        }
        usleep(50);
    }
}



int main()
{
    struct sockaddr_in server_addr, client_addr;
    int sock_server, sock_client;
    char *server_ip_addr = NULL;

    char menu = 's';
    SDL_Window *window;
    SDL_Renderer *renderer;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        cout << "Failed to load SDL \n";
    };
    SDL_Texture *tex = NULL;
   
    SDL_Texture *bullet = NULL;
     SDL_Texture *fire = NULL;
    SDL_Texture *power = NULL;
    SDL_Texture *map = NULL;
    SDL_Texture *build = NULL;
    SDL_Texture *bomb = NULL;
    TTF_Init();
    TTF_Font *font;
    font = TTF_OpenFont("resources/m5x7.ttf", 24);
    init_players();
    window = SDL_CreateWindow(
        "game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0);

    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Audio music;
    music.load("music/base.wav");
    music.play();
    
    if (renderer == NULL)
    {
        SDL_DestroyWindow(window);
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    tex = load_texture(renderer, "resources/player2.bmp");
    fire = load_texture(renderer, "resources/fire.bmp");
    bullet = load_texture(renderer, "resources/bullet.bmp");
    build = load_texture(renderer, "resources/building.bmp");
    power = load_texture(renderer, "resources/power2.png");
    bomb = load_texture(renderer, "resources/bomb.bmp");
    
    int i;
    server_or_client(renderer, &menu, font);
    if (menu == 'c')
    {
        server_ip_addr = static_cast<char *>(malloc(16 * sizeof(char)));
        ask_for_ip(renderer, font, server_ip_addr);
    }
    pthread_t thread_id_server, thread_id_client, thread_id_server_send;
    server_addr = server_sock_addr(server_ip_addr);
    client_addr = client_sock_addr();
    if (menu == 's')
    {
        prepare_server(&sock_server, &server_addr);
        pthread_create(&thread_id_server, NULL, server_receive_loop, &sock_server);
        pthread_create(&thread_id_server_send, NULL, server_send_loop, &sock_server);
    }
    prepare_client(&sock_client, &client_addr);
    pthread_create(&thread_id_client, NULL, client_loop, &sock_client);

    while (my_id < 0)
    {
        send_to_server(sock_client, server_addr, my_id, 0);
        usleep(100);
    }
    music.stop();

    SDL_Rect bullet_pos;
    bullet_pos.w = BULLET_HEIGHT;
    bullet_pos.h = BULLET_HEIGHT;
    map = get_map_texture(renderer);

    SDL_Event e;

    Audio effect;
    effect.load("music/pacman_intro.wav");

    while (running)
    {
         if(winner>=0){
           break;
        }
       
        effect.play();

        

        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                break;
            }
            resolve_keyboard(e, &players[my_id]);
        }
        

        send_to_server(sock_client, server_addr, my_id, key_state_from_player(&players[my_id]));
        usleep(30);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, map, NULL, NULL);
        for (i = 0; i <= number_of_players; i++)
        {
            SDL_RenderCopy(renderer, tex, NULL, &players[i].position);
        }

        disp_text(renderer, "kills", font, 400, 10);
        for (i = 0; i <= number_of_players; i++)
        {
            char kills[10] = {};
            sprintf(kills, "%d", players[i].kills);
            disp_text(renderer, kills, font, 400, 30 + i * 20);
        }

        disp_text(renderer, "wins", font, 460, 10);
        for (i = 0; i <= number_of_players; i++)
        {
            char deaths[10] = {};
            sprintf(deaths, "%d", players[i].wins);
            disp_text(renderer, deaths, font, 460, 30 + i * 20);
        }

        bullet_pos.w = BULLET_HEIGHT;
        bullet_pos.h = BULLET_HEIGHT;

        for (i = 0; i < bullets_number; i++)
        {
            bullet_pos.x = bullets_client[i * 2];
            bullet_pos.y = bullets_client[i * 2 + 1];
            
            SDL_RenderCopy(renderer, bullet, NULL, &bullet_pos);
        }
        // cout<<"power_array \n";
        bullet_pos.w = FIRE_HEIGHT;
        bullet_pos.h = FIRE_HEIGHT;
         for (i = 0; i <MAX_POWER; i++)
        {
            bullet_pos.x = power_array[i].first;
            bullet_pos.y = power_array[i].second;
            if(i%2==0){
SDL_RenderCopy(renderer, fire, NULL, &bullet_pos);
            }
            else{
                SDL_RenderCopy(renderer, bomb, NULL, &bullet_pos);
            }
            // cout<<power_array[i].first<<" "<<power_array[i].second<<endl;
            
        }


        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

         
    }
    SDL_RenderClear(renderer);
    // int i=0;.
    
    
    winningscreen(winner,renderer,font);
     usleep(3000000);
   
    close(sock_client);
    close(sock_server);
    pthread_cancel(thread_id_client);
    pthread_cancel(thread_id_server);
    pthread_cancel(thread_id_server_send);
    effect.stop();

   

    
    Mix_Quit();
    SDL_DestroyTexture(tex);
    SDL_DestroyTexture(bullet);
    SDL_DestroyTexture(map);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
