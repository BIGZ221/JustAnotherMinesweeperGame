#include "raylib.h"
#include "game.h"

void ClearIslandFromIndex(Game game, int row, int col);

Game NewGame(int width, int height, int mines)
{
    Cell *cells = (Cell *)malloc(width * height * sizeof(Cell));
    Game game = (Game){
        width,
        height,
        mines,
        false,
        cells,
    };
    ResetGame(game);
    return game;
}

void FreeGame(Game game)
{
    free(game.cells);
}

void ClearIsland(Game game, Cell *startingCell) {
    int startIndex;
    for (size_t i = 0; i < game.width * game.height; i++)
    {
        if (startingCell == &game.cells[i]) startIndex = i;
    }
    int startRow = startIndex / 10;
    int startCol = startIndex % 10;
    ClearIslandFromIndex(game, startRow, startCol);
}

void ClearIslandFromIndex(Game game, int row, int col)
{
    Cell *cell = &game.cells[row * game.height + col];
    cell->status = SHOWING;
    // Above
    int aboveIndex = (row - 1) * game.height + col;
    if (row > 0 && game.cells[aboveIndex].adjacentMines == 0 && game.cells[aboveIndex].status == HIDDEN)
    {
        ClearIslandFromIndex(game, row - 1, col);

    }
    // Below
    int belowIndex = (row + 1) * game.height + col;
    if (row < game.height - 1 && game.cells[belowIndex].adjacentMines == 0 && game.cells[belowIndex].status == HIDDEN)
    {
        ClearIslandFromIndex(game, row + 1, col);
    }
    // Left
    int leftIndex = row * game.height + col - 1;
    if (col > 0 && game.cells[leftIndex].adjacentMines == 0 && game.cells[leftIndex].status == HIDDEN)
    {
        ClearIslandFromIndex(game, row, col - 1);
    }
    // Right
    int rightIndex = row * game.height + col + 1;
    if (col < game.width - 1 && game.cells[rightIndex].adjacentMines == 0 && game.cells[rightIndex].status == HIDDEN)
    {
        ClearIslandFromIndex(game, row, col + 1);
    }
}

void ResetGame(Game game)
{
    game.isLost = false;
    int minesRemaining = game.mines;
    for (size_t i = 0; i < game.width * game.height; i++) {
        Cell *cell = &game.cells[i];
        cell->isMine = false;
        cell->status = HIDDEN;
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
