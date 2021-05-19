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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UP 0    //-y
#define DOWN 1  //+y
#define LEFT 2  //-x
#define RIGHT 3 //+x
#define OUTFILE "MAZE"
#define WHITE fprintf(outfile, "%c%c%c", 255, 255, 255)
#define BLACK fprintf(outfile, "%c%c%c", 0, 0, 0)
#define RED fprintf(outfile, "%c%c%c", 0, 0, 255)

#define nodeadend //generate a maze without any dead ends! (consequently, many solutions to maze)
//#define prim    //enable this to generate mazes using prim's algorithm.
#define backtrack //enable this to generate mazes using depth-first search. Don't enable both.
//#define movie   //this option spams bitmaps to illustrate each step of generation.

long numin = 1; //Number of cells in the maze.
const int xsize = 14;
const int ysize = 19;

void initialize();
void generate();
void savebmp(int xspecial, int yspecial);

struct cell
{
    bool in;          //Is this cell in the maze?
    bool up;          //Does the wall above this cell exist?
    bool left;        //Does the wall to the left of this cell exist?
    int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
};

cell MAZE[xsize][ysize];

void initialize()
{
    //Initialize the maze!
    for (int x = 0; x < xsize; x++)
    {
        for (int y = 0; y < ysize; y++)
        {
            //The maze cells on the edges of the maze are "in" to provide padding. Otherwise, all maze cells are not in.
            MAZE[x][y].in = (x == 0 || x == xsize - 1 || y == 0 || y == ysize - 1) ? 1 : 0;
            //All maze cells have all walls existing by default, except the perimeter cells.
            MAZE[x][y].up = (x == 0 || x == xsize - 1 || y == 0) ? 0 : 1;
            MAZE[x][y].left = (x == 0 || y == 0 || y == ysize - 1) ? 0 : 1;
        }
    }
    return;
}

void generate()
{
    int xcur = 1, ycur = 1; //start growing from the corner. It could theoretically start growing from anywhere, doesn't matter.
    MAZE[xcur][ycur].in = 1;
    int whichway;
    bool success;
    do
    {
#ifdef movie
        savebmp(xcur, ycur);
#endif
#ifdef nodeadend
        if (MAZE[xcur][ycur - 1].in && MAZE[xcur][ycur + 1].in &&
            MAZE[xcur - 1][ycur].in && MAZE[xcur + 1][ycur].in)
        {
            //If at a dead end, randomly destroy a wall to make it not a dead end!
            do
            {
                success = 0;
                whichway = rand() % 4;
                switch (whichway)
                {
                case UP:
                    if (MAZE[xcur][ycur].up && ycur != 1)
                    {
                        success = 1;
                        MAZE[xcur][ycur].up = 0;
                    }
                    break;
                case DOWN:
                    if (MAZE[xcur][ycur + 1].up && ycur != ysize - 2)
                    {
                        success = 1;
                        MAZE[xcur][ycur + 1].up = 0;
                    }
                    break;
                case LEFT:
                    if (MAZE[xcur][ycur].left && xcur != 1)
                    {
                        success = 1;
                        MAZE[xcur][ycur].left = 0;
                    }
                    break;
                case RIGHT:
                    if (MAZE[xcur + 1][ycur].left && xcur != xsize - 2)
                    {
                        success = 1;
                        MAZE[xcur + 1][ycur].left = 0;
                    }
                    break;
                }
            } while (!success);
        }
#endif
#ifdef backtrack
        while (MAZE[xcur][ycur - 1].in && MAZE[xcur][ycur + 1].in &&
               MAZE[xcur - 1][ycur].in && MAZE[xcur + 1][ycur].in)
        {
            //If all the neighbourhood cells are in, backtrack.
            int xcur2 = MAZE[xcur][ycur].prevx;
            ycur = MAZE[xcur][ycur].prevy;
            xcur = xcur2;
        }
#endif
#ifdef prim
        do
        {
            //randomly find a cell that's in the maze
            xcur = rand() % (xsize - 2) + 1;
            ycur = rand() % (ysize - 2) + 1;
        } while (!MAZE[xcur][ycur].in ||
                 MAZE[xcur][ycur - 1].in && MAZE[xcur][ycur + 1].in &&
                     MAZE[xcur - 1][ycur].in && MAZE[xcur + 1][ycur].in);
#endif
        do
        {
            //Randomly grow the maze if possible.
            success = 0;
            whichway = rand() % 4;
            switch (whichway)
            {
            case UP:
                if (!MAZE[xcur][ycur - 1].in)
                {
                    success = 1;
                    MAZE[xcur][ycur].up = 0;
                    MAZE[xcur][ycur - 1].prevx = xcur;
                    MAZE[xcur][ycur - 1].prevy = ycur;
                    ycur--;
                }
                break;
            case DOWN:
                if (!MAZE[xcur][ycur + 1].in)
                {
                    success = 1;
                    MAZE[xcur][ycur + 1].up = 0;
                    MAZE[xcur][ycur + 1].prevx = xcur;
                    MAZE[xcur][ycur + 1].prevy = ycur;
                    ycur++;
                }
                break;
            case LEFT:
                if (!MAZE[xcur - 1][ycur].in)
                {
                    success = 1;
                    MAZE[xcur][ycur].left = 0;
                    MAZE[xcur - 1][ycur].prevx = xcur;
                    MAZE[xcur - 1][ycur].prevy = ycur;
                    xcur--;
                }
                break;
            case RIGHT:
                if (!MAZE[xcur + 1][ycur].in)
                {
                    success = 1;
                    MAZE[xcur + 1][ycur].left = 0;
                    MAZE[xcur + 1][ycur].prevx = xcur;
                    MAZE[xcur + 1][ycur].prevy = ycur;
                    xcur++;
                }
                break;
            }
        } while (!success);
        MAZE[xcur][ycur].in = 1;
        numin++; //Every iteration of this loop, one maze cell is added to the maze.
    } while (numin < (xsize - 2) * (ysize - 2));
#ifdef movie
    savebmp(xcur, ycur);
#endif
    return;
}
int map[15][20];
void savebmp(int xspecial, int yspecial)
{
    //save a bitmap file! the xspecial, yspecial pixel is coloured red.
    FILE *outfile;
    int extrabytes, paddedsize;
    int x, y, n;
    int width = (xsize - 1) * 2 - 1;
    int height = (ysize - 1) * 2 - 1;

    extrabytes = (4 - ((width * 3) % 4)) % 4;

    char filename[200];

    sprintf(filename, "%s_%dx%d_n%d.bmp", OUTFILE, xsize, ysize, numin);
    paddedsize = ((width * 3) + extrabytes) * height;

    unsigned int headers[13] = {static_cast<unsigned int>(paddedsize) + 54, 0, 54, 40, static_cast<unsigned int>(width), static_cast<unsigned int>(height), 0, 0, static_cast<unsigned int>(paddedsize), 0, 0, 0, 0};

    outfile = fopen(filename, "wb");
    fprintf(outfile, "BM");

    // for (n = 0; n <= 5; n++){
    //    fprintf(outfile, "%c", headers[n] & 0x000000FF);
    //    fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
    //    fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
    //    fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
    // }

    // fprintf(outfile, "%c", 1);fprintf(outfile, "%c", 0);
    // fprintf(outfile, "%c", 24);fprintf(outfile, "%c", 0);

    // for (n = 7; n <= 12; n++){
    //    fprintf(outfile, "%c", headers[n] & 0x000000FF);
    //    fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
    //    fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
    //    fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
    // }

    //Actual writing of data begins here:
    for (y = 0; y <= height - 1; y++)
    {
        for (x = 0; x <= width - 1; x++)
        {
            if (x % 2 == 1 && y % 2 == 1)
            {
                if (x / 2 + 1 == xspecial && y / 2 + 1 == yspecial)
                    map[x][y] = 2;
                else
                {
                    if (MAZE[x / 2 + 1][y / 2 + 1].in)
                        map[x][y] = 0;
                    else
                        map[x][y] = 1;
                }
            }
            else if (x % 2 == 0 && y % 2 == 0)
            {
                map[x][y] = 1;
            }
            else if (x % 2 == 0 && y % 2 == 1)
            {
                if (MAZE[x / 2 + 1][y / 2 + 1].left)
                    map[x][y] = 1;
                else
                    map[x][y] = 0;
            }
            else if (x % 2 == 1 && y % 2 == 0)
            {
                if (MAZE[x / 2 + 1][y / 2 + 1].up)
                    map[x][y] = 1;
                else
                    map[x][y] = 0;
            }
        }
        if (extrabytes)
        { // See above - BMP lines must be of lengths divisible by 4.
            for (n = 1; n <= extrabytes; n++)
            {
                fprintf(outfile, "%c", 0);
            }
        }
    }
    // printf("file printed: %s\n", filename);
    // fclose(outfile);
    return;
}

// int map[15][20] = {
//     {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
//     {0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0},
//     {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//     {1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1},
//     {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
//     {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

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
        b->position.x += (PLAYER_SPEED / 10) * b->face * 1;
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

    if (map[p.y / TILE_SIZE][p.x / TILE_SIZE] == 2)
    {
        map[p.y / TILE_SIZE][p.x / TILE_SIZE] = 0;
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
    cout << "map updated \n";

    SDL_Surface *bitmap = NULL;
    SDL_Texture *map_texture;
    SDL_Rect rect;
    srand((unsigned int)time(NULL)); //seed random number generator with system time
    initialize();                    //initialize the maze
    generate();                      //generate the maze
#ifdef movie
    for (int i = 1; i < 10; i++)
    {
        numin++;
        savebmp(0, 0); //output the bitmap
    }
#else movie
    savebmp(0, 0);
#endif

    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            cout << map[i][j];
        }
        cout << endl;
    }

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
            else if (map[i][j] == 2)
            {
                rect.x = TILE_SIZE * j;
                rect.y = TILE_SIZE * i;
                SDL_RenderCopy(renderer, power, NULL, &rect);
            }
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
    return map_texture;
}
