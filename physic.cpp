#include <vector>
#include <time.h>
#include <set>
#include <utility>

#include "physic.hpp"

// #include "main.cpp"
// #include "audio.hpp"

// powerUps PlayerPower;
struct PowerUps playerPower[2];

#define GRID_WIDTH 19
#define GRID_HEIGHT 15
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

int map[15][20];


//----GLOBAL VARIABLES------------------------------------------------
char grid[GRID_WIDTH * GRID_HEIGHT];

//----FUNCTION PROTOTYPES---------------------------------------------
void ResetGrid();
int XYToIndex(int x, int y);
int IsInBounds(int x, int y);
void Visit(int x, int y);
void PrintGrid();
vector<pair<int, int>> freespace;
//----FUNCTIONS-------------------------------------------------------

void ResetGrid()
{
    // Fills the grid with walls ('#' characters).
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i)
    {
        grid[i] = '#';
    }
}
int XYToIndex(int x, int y)
{
    // Converts the two-dimensional index pair (x,y) into a
    // single-dimensional index. The result is y * ROW_WIDTH + x.
    return y * GRID_WIDTH + x;
}
int IsInBounds(int x, int y)
{
    // Returns "true" if x and y are both in-bounds.
    if (x < 0 || x >= GRID_WIDTH)
        return false;
    if (y < 0 || y >= GRID_HEIGHT)
        return false;
    return true;
}

void Visit(int x, int y)
{
    // Starting at the given index, recursively visits every direction in a
    // randomized order.
    // Set my current location to be an empty passage.
    grid[XYToIndex(x, y)] = ' ';
    // Create an local array containing the 4 directions and shuffle their order.
    int dirs[4];
    dirs[0] = NORTH;
    dirs[1] = EAST;
    dirs[2] = SOUTH;
    dirs[3] = WEST;
    for (int i = 0; i < 4; ++i)
    {
        int r = rand() & 3;
        int temp = dirs[r];
        dirs[r] = dirs[i];
        dirs[i] = temp;
    }
    // Loop through every direction and attempt to Visit that direction.
    for (int i = 0; i < 4; ++i)
    {
        // dx,dy are offsets from current location. Set them based
        // on the next direction I wish to try.
        int dx = 0, dy = 0;
        switch (dirs[i])
        {
        case NORTH:
            dy = -1;
            break;
        case SOUTH:
            dy = 1;
            break;
        case EAST:
            dx = 1;
            break;
        case WEST:
            dx = -1;
            break;
        }
        // Find the (x,y) coordinates of the grid cell 2 spots
        // away in the given direction.
        int x2 = x + (dx << 1);
        int y2 = y + (dy << 1);
        if (IsInBounds(x2, y2))
        {
            if (grid[XYToIndex(x2, y2)] == '#')
            {
                // (x2,y2) has not been visited yet... knock down the
                // wall between my current position and that position
                grid[XYToIndex(x2 - dx, y2 - dy)] = ' ';
                // Recursively Visit (x2,y2)
                Visit(x2, y2);
            }
        }
    }
}

void PrintGrid()
{
    // Displays the finished maze to the screen.
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            cout << grid[XYToIndex(x, y)];
            if (grid[XYToIndex(x, y)] == '#')
            {
                map[y][x] = 1;
            }
            else
            {
                map[y][x] = 0;
            }
        }
        cout << endl;
    }
}



vector<pair<int, int>> getPowerArray()
{
    vector<pair<int, int>> powerArray(MAX_POWER, make_pair(0, 0));

    for (int i = 0; i < MAX_POWER; i++)
    {
        powerArray[i].first = playerPower[1].powerX[i];
        powerArray[i].second = playerPower[1].powerY[i];
    }
    return powerArray;
}

void updatePowerArray(vector<pair<int, int>> t)
{
    for (int i = 0; i < MAX_POWER; i++)
    {
        playerPower[1].powerX[i] = t[i].first;
        playerPower[1].powerY[i] = t[i].second;
    }
    return;
}

vector<pair<int, int>> getMap()
{
    //     Solution s;
    return freespace;
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
        b->position.x += (PLAYER_SPEED)*b->face * 1;
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
int check_if_player_power(struct Player *player, vector<pair<int, int>> &power_server, int &po_ind)
{
    // struct node *next = *bullets;
    struct SDL_Rect p = player->position;
    SDL_Rect rect;
    bool a=false;

    for (int i = 0; i < power_server.size(); i++)
    {
        int x = power_server[i].first, y = power_server[i].second;
        // map[x / TILE_SIZE][y / TILE_SIZE] = 2;

        if (
            (p.x < (x + FIRE_HEIGHT) &&
             (p.x + p.w) > x &&
             p.y < (y + FIRE_HEIGHT) &&
             (p.y + p.h) > y)

        )
        {

            if (i % 2 == 0)
            {
                po_ind = 0;
            }
            power_server[i].first = 0;
            power_server[i].second = 0;

            // SDL_RenderCopy(renderer, , NULL, &rect);
            a=true;
        }
    }

    if (map[p.y / TILE_SIZE][p.x / TILE_SIZE] == 2)
    {
        map[p.y / TILE_SIZE][p.x / TILE_SIZE] = 0;
        a=true;
    }
    if (a==true){
        Mix_Music *gMusic = NULL;
        Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
        
        gMusic = Mix_LoadMUS( "music/Laser_Shoot.wav" );
        Mix_PlayMusic( gMusic, 1 );
    }

    return a;
}
bool check_if_player_reach(struct Player *player)
{
    struct SDL_Rect p = player->position;

    if (map[p.y / TILE_SIZE][p.x / TILE_SIZE] == 2 ||
        map[(p.y + p.h) / TILE_SIZE][p.x / TILE_SIZE] == 2 ||
        map[(p.y) / TILE_SIZE][(p.x + p.w) / TILE_SIZE] == 2 ||
        map[(p.y + p.h) / TILE_SIZE][(p.x + p.w) / TILE_SIZE] == 2)
    {

        cout << p.y / TILE_SIZE << "-- " << p.x / TILE_SIZE << endl;

        // SDL_RenderCopy(renderer, , NULL, &rect);
        return true;
    }
    return false;
}
void move_player(struct Player *player)
{
    int x_movement = 0;
    int y_movement = 0;

    int player_speed = PLAYER_SPEED;
    if (player->powerA)
    {
        player_speed = SLOW_PLAYER;
        cout << "slow downed player speed \n";
        cout << player->powerATime << endl;
        ;
    }

    if (player->left)
    {
        x_movement -= player_speed;
        player->face = -1;
    }
    if (player->right)
    {
        x_movement += player_speed;
        player->face = 1;
    }
    if (player->up)
    {
        y_movement -= player_speed;
    }
    if (player->down)
    {
        y_movement += player_speed;
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

    int alpha=1000;
    // cout << "Give seed for maze \n";
    // cin >> alpha;
    srand(alpha); // seed random number generator.
    ResetGrid();
    Visit(1, 1);
    PrintGrid();
    map[14][1] = 2;

    cout << "map updated \n";
    SDL_Surface *bitmap = NULL;

    SDL_Texture *map_texture;
    SDL_Rect rect;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;

    bitmap = SDL_LoadBMP("resources/tile1.bmp");

  SDL_Texture *tex = NULL;
    tex = SDL_CreateTextureFromSurface(renderer, bitmap);

    bitmap = SDL_LoadBMP("resources/building.bmp");
    SDL_Texture *build = NULL;
    build = SDL_CreateTextureFromSurface(renderer, bitmap);

    map_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderTarget(renderer, map_texture);
    int i, j;
    for (i = 0; i < GRID_HEIGHT; i++)
    {
        for (j = 0; j < GRID_WIDTH; j++)
        {
            cout << map[i][j];
            if (map[i][j] == 1)
            {
                rect.x = TILE_SIZE * j;
                rect.y = TILE_SIZE * i;
                SDL_RenderCopy(renderer, tex, NULL, &rect);
            }
            else if (map[i][j] == 2)
            {
                // cout<<i<<" "<<j<<endl;
                rect.x = TILE_SIZE * j;
                rect.y = TILE_SIZE * i;
                SDL_RenderCopy(renderer, build, NULL, &rect);
            }
            else{
                freespace.push_back(make_pair(i,j));
            }
        }
        cout << endl;
    }
    SDL_SetRenderTarget(renderer, NULL);
    return map_texture;
}
