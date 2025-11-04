/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include "game.h"
#include "constant.h"
#include "renderer.h"

void ProcessEvents(Game game, Event *events);

int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Just another Minesweeper game");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	int boardSize = 25;
	int mineCount = 5;
	Game game = NewGame(boardSize, boardSize, mineCount);

	PrintGame(game);

	// game loop
	while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		Event *events = RenderGame(game);

		ProcessEvents(game, events);

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);
	FreeGame(game);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

void ProcessEvents(Game game, Event *events)
{
	Event *start = events;
	while (start != NULL)
	{
		Event *event = start;
		printf("Got event %d\n", event->type);
		switch (event->type)
		{
		case CLICK_CELL:
			event->data->cell->status = SHOWING;
			if (event->data->cell->isMine) game.isLost = true;
			ClearIsland(game, event->data->cell);
			break;
		case FLAG_CELL:
			event ->data->cell->status = FLAGGED;
			break;
		case RESET:
			ResetGame(game);
			break;
		default:
			break;
		}
		start = event->next;
		FreeEvent(event);
	}
}
