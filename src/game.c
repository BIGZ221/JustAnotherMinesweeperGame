#include "raylib.h"
#include "game.h"

Game NewGame(uint64_t width, uint64_t height)
{
    Cell *cells = (Cell *)malloc(width * height * sizeof(Cell));
    return (Game){
        width,
        height,
        cells,
    };
}

void FreeGame(Game game)
{
    free(game.cells);
}
