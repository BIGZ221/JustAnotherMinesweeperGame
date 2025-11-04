#ifndef RENDERER_HEADER_H
#define RENDERER_HEADER_H

#include "raylib.h"
#include "game.h"

typedef enum EventType
{
    RESET = 0,
    CLICK_CELL = 1,
    FLAG_CELL = 2,
} EventType;

typedef union EventData
{
    Cell *cell;
} EventData;

typedef struct Event
{
    EventType type;
    EventData *data;
    struct Event *next;
} Event;

Event *RenderGame(Game game);

EventData *NewCellEventData(Cell *cell);
void FreeCellEventData(EventData *event);

Event *NewEvent(EventType type, EventData *data, Event *next);
void FreeEvent(Event *event);

#endif
