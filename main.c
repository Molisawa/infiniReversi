#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-integer-division"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include "reversi_game_engine.h"
#include "graphic_engine.h"
#include <malloc.h>


int main() {

    InitAudioDevice();

    Music music = LoadMusicStream("resources/background.mp3");
    Board board;

    PlayMusicStream(music);

    const int screenWidth = 1000;
    const int screenHeight = 800;

    ScreenFlag *screen = malloc(sizeof(ScreenFlag));
    *screen = MENU;

    ScreenFlag *lastScreen = malloc(sizeof(ScreenFlag));

    char filename[11] = "";
    int numOfChars = 0;
    int frameCounter = 0;
    Difficulty difficulty = EASY;
    int customBoardSize = 0;

    Piece pieceSelected = (Piece) {BLACK_PIECE};

    Slider slider;
    initSlider(&slider);

    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "Reversi");

    float SQUARE_SIZE = (float) screenHeight / (float) board.size;

    ScreenFeatures screenF;

    initScreenFeatures(&screenF, screenWidth, screenHeight, SQUARE_SIZE);

    Menu menu = getMenu(board, &screenF);
    MenuOptions menuOptions = getMenuOptions(&screenF);
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

        switch (*screen) {
            case MENU:
                UpdateMusicStream(music);
                MenuScreen(&screenF, frameCounter, menuOptions, screen, &board);
                break;
            case GAME:
                *lastScreen = GAME;
                if (canSkipBlackPiece(&board)) {
                    computerMove(&board);
                }
                PlayScreen(&board, menu, &screenF, screen, mouse, clicked);
                DrawFPS(10, 10);
                EndDrawing();
                break;
            case SAVE:
                ShowFileSaverScreen(&board, &screenF, filename, frameCounter, mouse, screen,
                                    &numOfChars, lastScreen);
                EndDrawing();
                break;
            case LOAD:
                LoadFileScreen(&board, &screenF, screen, &slider);
                EndDrawing();
                break;
            case EDITOR:
                *lastScreen = EDITOR;
                EditorScreen(&screenF, &board, &pieceSelected, screen);
                EndDrawing();
                break;
            case CONFIG_EDITOR:
                ConfigEditorScreen(&screenF, &board, screen, &customBoardSize);
                EndDrawing();
                break;
            case CONFIG_GAME:
                ConfigGameScreen(&screenF, &board, screen, &customBoardSize, &difficulty);
                EndDrawing();
                break;
        }

        //EndDrawing();
        //----------------------------------------------------------------------------------
    }

    free(screen);
    free(lastScreen);
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

