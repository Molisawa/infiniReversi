#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-integer-division"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include "reversi_game_engine.h"
#include "graphic_engine.h"
#include <raylib.h>
#include <malloc.h>


int main() {
    Board board;

    const int screenWidth = 1000;
    const int screenHeight = 800;

    ScreenFlag *screen = malloc(sizeof(ScreenFlag));

    *screen = GAME;

    char filename[11] = "";
    int numOfChars = 0;
    int frameCounter = 0;

    Slider slider;
    initSlider(&slider);
    initializeGame(&board, 8, HARD);
    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "Reversi");

    float SQUARE_SIZE = (float) screenHeight / (float) board.size;

    ScreenFeatures screenF;

    initScreenFeatures(&screenF, screenWidth, screenHeight, SQUARE_SIZE);

    Menu menu = getMenu(board, &screenF);

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        frameCounter = (frameCounter + 1) % 60;
        Vector2 mouse = GetMousePosition();
        int clicked = 0;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) clicked = 1;

        int key = GetKeyPressed();

        while (key > 0) {
            if ((key >= 32) && (key <= 125) && numOfChars < 10) {
                numOfChars++;
                filename[numOfChars - 1] = (char) key;
            }
            key = GetKeyPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            numOfChars--;
            if (numOfChars < 0) numOfChars = 0;
            filename[numOfChars] = '\0';

        }


        BeginDrawing();


        if (canSkipBlackPiece(&board)) {
            computerMove(&board);
        }

        switch (*screen) {
            case MENU:
                break;
            case GAME:
                DrawBackground(&board, menu, &screenF, screen, mouse, clicked);
                DrawFPS(10, 10);
                break;
            case SAVE:
                ShowFileSaver(&board, &screenF, filename, frameCounter, mouse, screen,
                              &numOfChars);
                break;
            case LOAD:
                LoadFile(&board, &screenF, screen, &slider);
                break;
            case EDITOR:
                break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();
    free(screen);
    destructBoard(&board);
    return 0;
}

