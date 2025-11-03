#ifndef GAME_HEADER_H
#define GAME_HEADER_H

#include <stdbool.h>
#include <stdint.h>
#include <malloc.h>
#include "raylib.h"

typedef struct Cell
{
    bool isMine;
    bool isHidden;
    int adjacentMines;
} Cell;

typedef struct Game
{
    int width;
    int height;
    int mines;
    Cell *cells;
} Game;

Game NewGame(int width, int height, int mines);
void FreeGame(Game game);

void ResetMines(Game game);

void PrintGame(Game game);

#endif
