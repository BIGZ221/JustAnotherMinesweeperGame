#ifndef GAME_HEADER_H
#define GAME_HEADER_H

#include <stdbool.h>
#include <stdint.h>
#include <malloc.h>
#include "raylib.h"

typedef enum CellStatus
{
    HIDDEN = 0,
    SHOWING = 1,
    FLAGGED = 2
} CellStatus;

typedef struct Cell
{
    bool isMine;
    CellStatus status;
    int adjacentMines;
} Cell;

typedef struct Game
{
    int width;
    int height;
    int mines;
    bool isLost;
    Cell *cells;
} Game;

Game NewGame(int width, int height, int mines);
void FreeGame(Game game);

void ResetGame(Game game);
void ClearIsland(Game game, Cell *startingCell);

void PrintGame(Game game);

#endif
