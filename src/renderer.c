#include "raylib.h"
#include "constant.h"
#include "renderer.h"
#include <string.h>

#define HEADER_HEIGHT 100
#define GAME_HEIGHT WINDOW_HEIGHT - HEADER_HEIGHT
#define FONT_SIZE 20
#define BUTTON_HEIGHT 26
#define BUTTON_PADDING_X 4
#define BUTTON_PADDING_Y 4
#define BUTTON_ROUNDNESS 1.0
#define BUTTON_SEGMENTS 8

typedef struct Button
{
    Vector2 position;
    char *text;
} Button;

Event *RenderHeader(Game game);
Event *RenderBoard(Game game);
Event *RenderButton(Button button);
Event *ConcatEvents(Event *parentEvent, Event *childEvent);
int MeasureButton(Button button);

Event *RenderGame(Game game)
{
    Event *headerEvents = RenderHeader(game);
    Event *boardEvents = RenderBoard(game);
    return ConcatEvents(headerEvents, boardEvents);
}

Event *RenderHeader(Game game)
{
    DrawRectangle(0, 0, WINDOW_WIDTH, HEADER_HEIGHT, DARKGRAY);
    DrawRectangleLinesEx((Rectangle){0, 0, WINDOW_WIDTH, HEADER_HEIGHT}, 4.0, BLACK);
    Button resetButton = (Button){
        (Vector2){10, 10}, "Reset"};
    Event *buttonEvent = RenderButton(resetButton);
    DrawText(TextFormat("Mines %d", game.mines), 10, BUTTON_HEIGHT + 16, FONT_SIZE, BLACK);
    if (game.status != ONGOING)
    {
        int resetButtonWidth = MeasureButton(resetButton);
        const char *text = game.status == LOST ? "You lost!" : "You won!";
        DrawText(TextFormat(text), 20 + resetButtonWidth, 12, 20, BLACK);
    }
    return buttonEvent;
}

Event *RenderBoard(Game game)
{
    DrawRectangle(0, HEADER_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - HEADER_HEIGHT, GRAY);
    int cellDimension = WINDOW_WIDTH / game.width;
    int xOffset = (WINDOW_WIDTH - cellDimension * game.width) / 2;
    int boardWidth = cellDimension * game.width;
    int boardHeight = cellDimension * game.height;

    Event *event = NULL;
    for (size_t i = 0; i < game.height; i++)
    {
        for (size_t j = 0; j < game.width; j++)
        {
            Cell *cell = &game.cells[i * game.height + j];
            int topLeftX = xOffset + j * cellDimension;
            int topLeftY = HEADER_HEIGHT + i * cellDimension;
            Rectangle cellRect = (Rectangle){topLeftX, topLeftY, cellDimension, cellDimension};
            DrawRectangleLinesEx(cellRect, 1.0, BLACK);

            if (cell->status == HIDDEN)
            {
                DrawLine(cellRect.x + cellDimension / 4, cellRect.y + cellDimension / 2, cellRect.x + 3 * cellDimension / 4, cellRect.y + cellDimension / 2, BLACK);
            }
            else if (cell->status == FLAGGED)
            {
                const char *cellText = "?";
                int fontSize = cellDimension - 4;
                int textWidth = MeasureText(cellText, fontSize);
                DrawText(cellText, topLeftX + cellDimension / 2 - textWidth / 2, topLeftY + cellDimension / 2 - fontSize / 2, fontSize, BLACK);
            }
            else if (cell->isMine)
            {
                DrawCircle(topLeftX + cellDimension / 2, topLeftY + cellDimension / 2, cellDimension / 2, BLACK);
            }
            else if (cell->adjacentMines > 0)
            {
                const char *cellText = TextFormat("%d", cell->adjacentMines);
                int fontSize = cellDimension - 4;
                int textWidth = MeasureText(cellText, fontSize);
                DrawText(cellText, topLeftX + cellDimension / 2 - textWidth / 2, topLeftY + cellDimension / 2 - fontSize / 2, fontSize, BLACK);
            }
            if (CheckCollisionPointRec(GetMousePosition(), cellRect))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    EventData *eventData = NewCellEventData(cell);
                    event = NewEvent(CLICK_CELL, eventData, NULL);
                }
                else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
                {
                    EventData *eventData = NewCellEventData(cell);
                    event = NewEvent(FLAG_CELL, eventData, NULL);
                }
            }
        }
    }
    return event;
}

Event *RenderButton(Button button)
{
    int buttonWidth = MeasureText(button.text, FONT_SIZE);
    Rectangle buttonRect = (Rectangle){button.position.x, button.position.y, buttonWidth + 2 * BUTTON_PADDING_X, BUTTON_HEIGHT};
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), buttonRect);
    Color backgroundColor = isHovered ? DARKGRAY : GRAY;
    DrawRectangleRounded(buttonRect, BUTTON_ROUNDNESS, BUTTON_SEGMENTS, backgroundColor);
    DrawRectangleRoundedLinesEx(buttonRect, BUTTON_ROUNDNESS, BUTTON_SEGMENTS, 2.0, BLACK);
    DrawText(button.text, button.position.x + BUTTON_PADDING_X, button.position.y + BUTTON_PADDING_Y, FONT_SIZE, BLACK);
    if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return NewEvent(RESET, NULL, NULL);
    }
    return NULL;
}

int MeasureButton(Button button)
{
    int buttonWidth = MeasureText(button.text, FONT_SIZE);
    return buttonWidth + 2 * BUTTON_PADDING_X;
}

Event *ConcatEvents(Event *parentEvent, Event *childEvent)
{
    if (parentEvent == NULL)
        return childEvent;

    Event *top = parentEvent;
    while (top->next != NULL)
    {
        top = top->next;
    }
    top->next = childEvent;
    return parentEvent;
}

EventData *NewCellEventData(Cell *cell)
{
    EventData *data = (EventData *)malloc(sizeof(EventData));
    data->cell = cell;
    return data;
}

void FreeCellEventData(EventData *data)
{
    free(data);
}

Event *NewEvent(EventType type, EventData *data, Event *next)
{
    Event *event = (Event *)malloc(sizeof(Event));
    event->type = type;
    event->data = data;
    event->next = next;
    return event;
}

void FreeEvent(Event *event)
{
    event->next = NULL;
    if (event->type == CLICK_CELL || event->type == FLAG_CELL || event->data != NULL)
    {
        FreeCellEventData(event->data);
    }
    free(event);
}
