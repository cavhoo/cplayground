#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

time_t t;

int tickRate = 30; // 15fps tick in which the tile is moved to the bottom
int gridSize = 360;
int tileSize = 32;
int grid[360];
int nextTile = 0;
int tileIndexes[4] = {0, 0, 0, 0};
int tileRotationState = 0;
bool spawnNextTile = false;
bool redraw = false;

enum Tiles
{
EMPTY,
BEAM,
SQUARE,
LTILE,
STILE,
TTILE
};

enum Direction
{
DOWN,
LEFT,
RIGHT
};

enum GameState
{
MENU,
START,
GAME,
END
};

ALLEGRO_TIMER* timer;
ALLEGRO_DISPLAY* disp;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP* baseTile;

ALLEGRO_COLOR SquareTileColor;
ALLEGRO_COLOR LTileColor;
ALLEGRO_COLOR STileColor;
ALLEGRO_COLOR BeamColor;
ALLEGRO_COLOR TTileColor;

int pickNextTile()
{
    return LTILE; // rand() % 4 + 1;
}

void spawnTile(int tileType)
{
    printf("Spawning Tile: %d\n", tileType);
    tileRotationState = 0;
    switch(tileType)
    {
        case BEAM:
            tileIndexes[0] = 7;
            tileIndexes[1] = 7 + 15;
            tileIndexes[2] = 7 + 30;
            tileIndexes[3] = 7 + 45;
            break;
        case LTILE:
            tileIndexes[0] = 7;
            tileIndexes[1] = 7 + 15;
            tileIndexes[2] = 7 + 30;
            tileIndexes[3] = 7 + (nextTile == LTILE ? 31 : -31);
            break;
        case TTILE:
            tileIndexes[0] = 6;
            tileIndexes[1] = 7;
            tileIndexes[2] = 8;
            tileIndexes[3] = 7 + 15;
            break;
        case STILE:
            tileIndexes[0] = 6;
            tileIndexes[1] = 7;
            tileIndexes[2] = 7 + 15;
            tileIndexes[3] = 7 + 15 + 1;
            break;
        case SQUARE:
            tileIndexes[0] = 7;
            tileIndexes[1] = 8;
            tileIndexes[2] = 7 + 15;
            tileIndexes[3] = 8 + 15;
            break;
    }
}

void renderTile(int gridX, int gridY, ALLEGRO_COLOR tint)
{
    al_draw_tinted_bitmap(baseTile, tint, tileSize * gridX, tileSize * gridY, 0);
}

bool isIndexFree(int index)
{
    return grid[index] == 0;
}

bool checkForCollision(int indecesToCheck[], int indexCount,int offsetToCheck)
{
    bool collided = false;
    for (int i = 0; i < indexCount; i++)
    {
        printf("Checking Index %d\n", tileIndexes[indecesToCheck[i]]);
        if (!isIndexFree(tileIndexes[indecesToCheck[i]] + offsetToCheck)) collided = true;
    }
    printf("Collision: %s\n", collided ? "true" : "false");
    return collided;
}

bool LTileTouches(int direction)
{
    printf("Checking L Tile Collision");
    switch(direction)
    {
        case DOWN:
        {
            if (tileRotationState == 0)
            {
                int indexes[2] = {2, 3};
                return checkForCollision(indexes, 2, 15);
            }
            if (tileRotationState == 1)
            {
                int indexes[3] = {3, 0, 1};
                return checkForCollision(indexes, 3, 15);
            }
            if (tileRotationState == 2)
            {
                int indexes[2] = {0, 3};
                return checkForCollision(indexes, 2, 15);
            }
            if (tileRotationState == 3)
            {
                int indexes[3] = {1, 2, 3};
                return checkForCollision(indexes, 3, 15);
            }
            break;
        }
        case LEFT:
        {
            if (tileRotationState == 0)
            {
                int indexes[3] = {0, 1, 2};
                return checkForCollision(indexes, 3, -1);
            }
            if (tileRotationState == 1)
            {
                int indexes[2] = {2, 3};
                return checkForCollision(indexes, 2, -1);
            }
            if (tileRotationState == 2)
            {
                int indexes[3] = {3, 1, 0};
                return checkForCollision(indexes, 3, -1);
            }
            if (tileRotationState == 3)
            {
                int indexes[2] = {0, 3};
                return checkForCollision(indexes, 2, -1);
            }
        }
        case RIGHT:
        {
            if (tileRotationState == 0)
            {
                int indexes[3] = {0, 1, 3};
                return checkForCollision(indexes, 3, 1);
            }
            if (tileRotationState == 1)
            {
                int indexes[2] = {0, 3};
                return checkForCollision(indexes, 2, 1);
            }
            if (tileRotationState == 2)
            {
                int indexes[3] = {0, 1, 2};
                return checkForCollision(indexes, 3, 1);
            }
            if (tileRotationState == 3)
            {
                int indexes[2] = {2, 3};
                return checkForCollision(indexes, 2, 1);
            }
        }
    }
    return false;
}

bool SquareTileTouches(int direction)
{
    printf("Checking Square collision");
    switch(direction)
    {
        case DOWN:
        {
            int indexes[2] = {2, 3};
            return checkForCollision(indexes, 2, 15);
        }
        case LEFT:
        {
            int indexes[2] = {0, 2};
            return checkForCollision(indexes, 2, -1);
        }
        case RIGHT:
        {
            int indexes[2] = {1, 3};
            return checkForCollision(indexes, 2, 1);
        }

    }
    return false;
}

bool ZTileTouches(int direction)
{
    printf("Checking Z Tile Collision");
    switch(direction)
    {
        case DOWN:
        {
            int indexes[3] = {0, 3, 2};
            return checkForCollision(indexes, 3, 15);
        }
        case LEFT:
        {
            int indexes[2] = {0, 2};
            return checkForCollision(indexes, 2, -1);
        }
        case RIGHT:
        {
            int indexes[2] = {1, 3};
            return checkForCollision(indexes, 2, 1);
        }
    }
    return false;
}

bool BeamTouches(int direction)
{
    printf("Checking Beam Tile Collision");
    switch(direction)
    {
        case DOWN:
        {
            if (tileRotationState % 2 == 0)
            {
                int indexes[1] = {3};
                return checkForCollision(indexes, 1, 15);
            }
            else
            {
                int indexes[4] = {0, 1, 2, 3};
                return checkForCollision(indexes, 4, 15);
            }
            printf("Beam downward check");
        }
        case LEFT:
        {
            if (tileRotationState % 2 == 0)
            {
                int indexes[4] = {0, 1, 2, 3};
                return checkForCollision(indexes, 4, -1);
            }
            else
            {
                int indexes[1] = {3};
                return checkForCollision(indexes, 1, -1);
            }
        }
        case RIGHT:
        {
            if (tileRotationState % 2 == 0)
            {
                int indexes[4] = {0, 1, 2, 3};
                return checkForCollision(indexes, 4, 1);
            }
            else
            {
                int indexes[1] = {0};
                return checkForCollision(indexes, 1, 1);
            }
        }

    }
    return false;
}

bool TTileTouches(int direction)
{
    printf("Checking T Tile collision");
    switch(direction)
    {
        case DOWN:
        {
            int indexes[3] = {0, 3, 2};
            return checkForCollision(indexes, 3, 15);
        }
        case LEFT:
        {
            int indexes[2] = {0, 3};
            return checkForCollision(indexes, 2, -1);
        }
        case RIGHT:
        {
            int indexes[2] = {2, 3};
            return checkForCollision(indexes, 2, 1);
        }
    }
    return false;
}

bool hasReachedEnd()
{
    if(
        tileIndexes[0] / 15 == 23 ||
        tileIndexes[1] / 15 == 23 ||
        tileIndexes[2] / 15 == 23 ||
        tileIndexes[3] / 15 == 23
    )
    {
        return true;
    }
    return false;
}

bool willTouchTiles(int direction)
{
    switch (nextTile)
    {
        case BEAM:
            return BeamTouches(direction);
            break;
        case SQUARE:
            return SquareTileTouches(direction);
            break;
        case LTILE:
            return LTileTouches(direction);
            break;
        case TTILE:
            return TTileTouches(direction);
            break;
        case STILE:
            return ZTileTouches(direction);
            break;
    }
    return false;
}

void rotateCurrentTile()
{
    switch(nextTile)
    {
        case BEAM:
            if (tileRotationState % 2 == 0)
            {
                grid[tileIndexes[0]] = 0;
                tileIndexes[0] = tileIndexes[1] - 15;
                grid[tileIndexes[2]] = 0;
                tileIndexes[2] = tileIndexes[1] + 15;
                grid[tileIndexes[3]] = 0;
                tileIndexes[3] = tileIndexes[1] + 30;
            }
            else
            {
                grid[tileIndexes[0]] = 0;
                tileIndexes[0] = tileIndexes[1] + 1;
                grid[tileIndexes[2]] = 0;
                tileIndexes[2] = tileIndexes[1] - 1;
                grid[tileIndexes[3]] = 0;
                tileIndexes[3] = tileIndexes[1] - 2;
            }
            break;
        case SQUARE:
            return;
        case LTILE:
            if (tileRotationState == 0)
            {
                grid[tileIndexes[0]] = 0;
                tileIndexes[0] = tileIndexes[1] - 1;
                grid[tileIndexes[2]] = 0;
                tileIndexes[2] = tileIndexes[1] + 1;
                grid[tileIndexes[3]] = 0;
                tileIndexes[3] = tileIndexes[1] + 2;
            }
            if (tileRotationState == 1)
            {

            }

    }
    redraw = true;
}

void updateCurrentTilePosition(int direction, int currentTile)
{
    if ((tileIndexes[0] % 15 == 0 ||
        tileIndexes[1] % 15 == 0 ||
        tileIndexes[2] % 15 == 0 ||
        tileIndexes[3] % 15 == 0) && direction == 1)
    {
        return;
    }

    if ((tileIndexes[0] % 15 == 14 ||
        tileIndexes[1] % 15 == 14 ||
        tileIndexes[2] % 15 == 14 ||
        tileIndexes[3] % 15 == 14) && direction == 2
    )
    {
        return;
    }
    if (willTouchTiles(direction)) {
        if (direction == DOWN) {
            nextTile = pickNextTile(); //rand() % 4 + 1;
            spawnTile(nextTile);
        }
        else
        {
            return;
        }
    }
    // Reset current index to 0=empty
    grid[tileIndexes[0]] = 0;
    grid[tileIndexes[1]] = 0;
    grid[tileIndexes[2]] = 0;
    grid[tileIndexes[3]] = 0;

    switch(direction)
    {
        case DOWN:
            tileIndexes[0] += 15;
            tileIndexes[1] += 15;
            tileIndexes[2] += 15;
            tileIndexes[3] += 15;
            break;
        case LEFT:
            tileIndexes[0] += -1;
            tileIndexes[1] += -1;
            tileIndexes[2] += -1;
            tileIndexes[3] += -1;
            break;
        case RIGHT:
            tileIndexes[0] += 1;
            tileIndexes[1] += 1;
            tileIndexes[2] += 1;
            tileIndexes[3] += 1;
            break;
    }
    // Update the new indexes with tile type
    grid[tileIndexes[0]] = currentTile;
    grid[tileIndexes[1]] = currentTile;
    grid[tileIndexes[2]] = currentTile;
    grid[tileIndexes[3]] = currentTile;

    if (hasReachedEnd())
    {
        nextTile = pickNextTile();//rand() % 4 + 1;
        spawnTile(nextTile);
    }
}

int init()
{
    srand((unsigned) time(&t));
    if (!al_init())
    {
        printf("couldn't initialize allegro\n");
        return 1;
    }
    if (!al_install_keyboard())
    {
        printf("couldn't initialize keyboard\n");
        return 1;
    }
    if (!al_init_image_addon())
    {
        printf("couldn't initialize image addon\n");
        return 1;
    }
    timer = al_create_timer(1.0 / 30.0);
    if (!timer)
    {
        printf("Couldn't initialize timer\n");
        return 1;
    }
    queue = al_create_event_queue();
    if (!queue)
    {
        printf("couldn't initialize queue\n");
        return 1;
    }
    disp = al_create_display(1024, 768);
    if (!disp)
    {
        printf("couldn't init display\n");
        return 1;
    }
    font = al_create_builtin_font();
    if(!font)
    {
        printf("couldn't init fonts\n");
        return 1;
    }
    baseTile = al_load_bitmap("assets/tile.bmp");
    for(int i = 0; i < gridSize; i++) {
     grid[i] = 0;
    }
    SquareTileColor = al_map_rgb(115, 209, 208);
    LTileColor = al_map_rgb(170, 115, 209);
    STileColor = al_map_rgb(151, 191, 31);
    BeamColor = al_map_rgb(252, 141, 50);
    TTileColor = al_map_rgb(48,252,3);
    return 0;
}

ALLEGRO_COLOR getTileColor() {
    switch(nextTile)
    {
        case BEAM:
            return BeamColor;
        case LTILE:
            return LTileColor;
        case TTILE:
            return TTileColor;
        case STILE:
            return STileColor;
        case SQUARE:
            return SquareTileColor;
    }
    return SquareTileColor;
}

int main()
{
    if(init() == 1)
    {
        printf("Initialization failed\n");
        return 1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
   
    nextTile = pickNextTile(); // Tile number drawn

    int frames = 0;
    ALLEGRO_EVENT event;
    spawnTile(nextTile);
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                frames++;
                if (frames % tickRate == 0) {
                    frames = 0;
                    updateCurrentTilePosition(DOWN, nextTile);
                }
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                {
                    updateCurrentTilePosition(LEFT, nextTile);
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                {
                    updateCurrentTilePosition(RIGHT, nextTile);
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    done = true;
                    break;
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
                {
                    if (tileRotationState < 3) {
                        tileRotationState += 1;
                    }
                    else
                    {
                        tileRotationState = 0;
                    }
                    rotateCurrentTile();
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if (done)
            break;
        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            for(int i = 0; i < 4; i++)
            {
                renderTile(tileIndexes[i] % 15, tileIndexes[i] / 15, getTileColor());
            }
            for(int i = 0; i < gridSize; i++) {
                int x = i % 15;
                int y = i / 15;
                switch(grid[i]) {
                    case SQUARE: // Square Tile
                        renderTile(x, y, SquareTileColor);
                        break;
                    case LTILE:
                        renderTile(x, y, LTileColor);
                        break;
                    case STILE:
                        renderTile(x, y, STileColor);
                        break;
                    case BEAM:
                        renderTile(x, y, BeamColor);
                        break;
                    case TTILE:
                        renderTile(x, y, TTileColor);
                        break;
                }
            }
            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
