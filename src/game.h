#include <stdbool.h>
#include <stdint.h>
#include <malloc.h>

#include "raylib.h"

typedef struct Cell
{
    bool isMine;
    bool isHidden;
    uint64_t adjacentMines;
} Cell;

typedef struct Game
{
    uint64_t width;
    uint64_t height;
    Cell *cells;
} Game;

Game NewGame(uint64_t width, uint64_t height);
void FreeGame(Game game);
