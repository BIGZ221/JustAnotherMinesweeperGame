#include "raylib.h"
#include "game.h"

Game NewGame(int width, int height, int mines)
{
    Cell *cells = (Cell *)malloc(width * height * sizeof(Cell));
    Game game = (Game){
        width,
        height,
        mines,
        cells,
    };
    ResetMines(game);
    return game;
}

void FreeGame(Game game)
{
    free(game.cells);
}

void ResetMines(Game game)
{
    int minesRemaining = game.mines;
    for (size_t i = 0; i < game.width * game.height; i++) {
        Cell *cell = &game.cells[i];
        cell->isMine = false;
        cell->isHidden = true;
    }
    int *minePositions = LoadRandomSequence(game.mines, 0, game.width * game.height -1);
    for (size_t i = 0; i < game.mines; i++)
    {
        Cell *cell = &game.cells[minePositions[i]];
        if (!cell->isMine)
        {
            cell->isMine = true;
            minesRemaining--;
        }
    }
    UnloadRandomSequence(minePositions);
    for (size_t i = 0; i < game.height; i++)
    {
        for (size_t j = 0; j < game.width; j++)
        {
            Cell *cell = &game.cells[i * game.height + j];
            cell->adjacentMines = 0;
            // Above
            if (i > 0 && game.cells[(i - 1) * game.height + j].isMine)
            {
                cell->adjacentMines++;
            }
            // Below
            if (i < game.height - 1 && game.cells[(i + 1) * game.height + j].isMine)
            {
                cell->adjacentMines++;
            }
            // Left
            if (j > 0 && game.cells[i * game.height + j - 1].isMine)
            {
                cell->adjacentMines++;
            }
            // Right
            if (j < game.width - 1 && game.cells[i * game.height + j + 1].isMine)
            {
                cell->adjacentMines++;
            }
            // Top left
            if (i > 0 && j > 0 && game.cells[(i - 1) * game.height + j - 1].isMine)
            {
                cell->adjacentMines++;
            }
            // Top right
            if (i > 0 && j < game.width - 1 && game.cells[(i - 1) * game.height + j + 1].isMine)
            {
                cell->adjacentMines++;
            }
            // Bottom left
            if (i < game.height - 1 && j > 0 && game.cells[(i + 1) * game.height + j - 1].isMine)
            {
                cell->adjacentMines++;
            }
            // Bottom right
            if (i < game.height - 1 && j < game.width - 1 && game.cells[(i + 1) * game.height + j + 1].isMine)
            {
                cell->adjacentMines++;
            }
        }
    }
}

void PrintGame(Game game)
{
    int width = game.width;
    printf("|%dX%d %d|\n", game.width, game.width, game.mines);
    for (size_t i = 0; i < game.height; i++)
    {
        for (size_t j = 0; j < game.width; j++)
        {
            Cell cell = game.cells[i * game.height + j];
            if (cell.isMine) {
                printf("X");
            } else {
                printf("%d", cell.adjacentMines);
            }
        }
        printf("\n");
    }
}
