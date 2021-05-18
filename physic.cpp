#include <vector>
#include <time.h>
#include <set>
#include <utility>

#include "physic.hpp"
#include "constans.hpp"
#include "maze.hpp"
#include "audio.hpp"
// #include "main.cpp"
// #include "audio.hpp"

int map[15][20] = {
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

vector<pair<int, int>> freespace;

void getMap(vector<pair<int, int>> &freespace)
{
    //     Solution s;
    int height = 15;
    int width = 20;
    //     srand(time(0));
    //     vector<int> row(width);
    //     vector<vector<int>> maze;
    //     for (int i = 0; i < height; ++i)
    //     {
    //         maze.push_back(row);
    //     }
    //     s.maze(maze);
    //     s.showMaze(maze);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (map[i][j] == 0)
            {
                freespace.push_back(make_pair(i, j));
            }
        }
    }
}

int sign(int a)
{
    if (a < 0)
    {
        return -1;
    }
    else if (a > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void decrement_abs(int *a)
{
    *a -= sign(*a);
}

int check_collisions(SDL_Rect *rect)
{
    if (map[rect->y / TILE_SIZE][rect->x / TILE_SIZE] == 1 ||
        map[(rect->y + rect->h) / TILE_SIZE][rect->x / TILE_SIZE] == 1 ||
        map[(rect->y) / TILE_SIZE][(rect->x + rect->w) / TILE_SIZE] == 1 ||
        map[(rect->y + rect->h) / TILE_SIZE][(rect->x + rect->w) / TILE_SIZE] == 1 ||
        rect->x <= 0 || (rect->x + rect->w >= SCREEN_WIDTH))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int move_and_check_collisions(SDL_Rect *position, int axis, int mov)
{
    SDL_Rect temp = *position;

    if (axis == X_AXIS)
    {
        temp.x += sign(mov);
    }

    if (axis == Y_AXIS)
    {
        temp.y += sign(mov);
    }

    if (check_collisions(&temp))
    {
        return 0;
    }
    else
    {
        *position = temp;
        return 1;
    }
}

void move_bullets(struct node **bullets)
{
    struct node *next = *bullets;
    struct Bullet *b = NULL;
    int i = 0;
    while (next != NULL)
    {
        b = (struct Bullet *)next->data;
        b->position.x += (PLAYER_SPEED/10) * b->face * 1;
        next = next->next;
        if (check_collisions(&b->position))
        {
            erase_element(bullets, i);
        }
        else
        {
            i++;
        }
    }
}

int check_if_player_dies(struct Player *player, struct node **bullets, int *killer)
{
    // effect.load("./music/laser_Shoot.bfxrsound");
    struct node *next = *bullets;
    struct SDL_Rect b;
    struct SDL_Rect p = player->position;
    int i = 0;
    while (next != NULL)
    {
        b = ((struct Bullet *)next->data)->position;
        if (p.x < (b.x + b.w) &&
            (p.x + p.w) > b.x &&
            p.y < (b.y + b.h) &&
            (p.y + p.h) > b.y)
        {
            *killer = ((struct Bullet *)next->data)->player_id;
            erase_element(bullets, i);
            //  Mix_PlayChannel(gMusic, -1,0);
            // play_music("./music/laser_Shoot.bfxrsound");
            // effect.play();
            return true;
        }
        next = next->next;
        i++;
    }
    return false;
}
int check_if_player_power(struct Player *player, set<pair<int, int>> &power_server)
{
    // struct node *next = *bullets;
    struct SDL_Rect p = player->position;
    SDL_Rect rect;

    // for (auto i : power_server)
    // {
    //     int x = i.first, y = i.second;
    //     map[x / TILE_SIZE][y / TILE_SIZE] = 2;

    //     if (map[p.y / TILE_SIZE][p.x / TILE_SIZE] == 2 ||
    //         map[(p.y + p.h) / TILE_SIZE][p.x / TILE_SIZE] == 2 ||
    //         map[(p.y) / TILE_SIZE][(p.x + p.w) / TILE_SIZE] == 2 ||
    //         map[(p.y + p.h) / TILE_SIZE][(p.x + p.w) / TILE_SIZE] == 2

    //     )
    //     {

    //         map[p.y / TILE_SIZE][p.x / TILE_SIZE] = 0;
    //         rect.x = TILE_SIZE * (p.y / TILE_SIZE);
    //         rect.y = TILE_SIZE * (p.x / TILE_SIZE);
    //         power_server.erase(i);
    //         // SDL_RenderCopy(renderer, , NULL, &rect);
    //         return true;
    //     }
    // }

     if (map[p.y / TILE_SIZE][p.x / TILE_SIZE] == 2 
        )
        { 
            map[p.y / TILE_SIZE][p.x / TILE_SIZE] =0;
            return true;
        }


    return false;
}

void move_player(struct Player *player)
{
    int x_movement = 0;
    int y_movement = 0;
    if (player->left)
    {
        x_movement -= PLAYER_SPEED;
        player->face = -1;
    }
    if (player->right)
    {
        x_movement += PLAYER_SPEED;
        player->face = 1;
    }
    if (player->up)
    {
        y_movement -= PLAYER_SPEED;
    }
    if (player->down)
    {
        y_movement += PLAYER_SPEED;
    }

    // y_movement = player->y_speed / 3;
    // if (player->y_speed < MAX_VERTICAL_SPEED) {
    //     player->y_speed += GRAVITY;
    // }

    while (x_movement != 0 || y_movement != 0)
    {
        if (x_movement != 0 && move_and_check_collisions(&player->position, X_AXIS, x_movement))
        {
            decrement_abs(&x_movement);
        }
        else
        {
            x_movement = 0;
        }

        if (y_movement != 0 && move_and_check_collisions(&player->position, Y_AXIS, y_movement))
        {
            decrement_abs(&y_movement);
        }
        else
        {
            y_movement = 0;
        }
    }
}

SDL_Texture *get_map_texture(SDL_Renderer *renderer)
{
    cout<<"map updated \n";
    SDL_Surface *bitmap = NULL;
    SDL_Texture *map_texture;
    SDL_Rect rect;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
    bitmap = SDL_LoadBMP("resources/tile1.bmp");
    SDL_Texture *tex = NULL;
    tex = SDL_CreateTextureFromSurface(renderer, bitmap);

    bitmap = SDL_LoadBMP("resources/bullet.bmp");
    SDL_Texture *power = NULL;
    power = SDL_CreateTextureFromSurface(renderer, bitmap);

    map_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderTarget(renderer, map_texture);
    int i, j;
    for (i = 0; i < SCREEN_HEIGHT / TILE_SIZE; i++)
    {
        for (j = 0; j < SCREEN_WIDTH / TILE_SIZE; j++)
        {
            if (map[i][j] == 1)
            {
                rect.x = TILE_SIZE * j;
                rect.y = TILE_SIZE * i;
                SDL_RenderCopy(renderer, tex, NULL, &rect);
            }
            else if(map[i][j]==2){
                rect.x = TILE_SIZE * j;
                rect.y = TILE_SIZE * i;
                SDL_RenderCopy(renderer, power, NULL, &rect);

            }
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
    return map_texture;
}
