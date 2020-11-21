#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-integer-division"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include "reversi_game_engine.h"
#include <raylib.h>
#include <malloc.h>
#include <math.h>
#include <dirent.h>
#include <stdio.h>

typedef struct menu {
    Rectangle goBackButton, goFowardButton, saveGameButton, loadGameButton;
} Menu;

typedef enum {
    MENU, GAME, SAVE, LOAD, EDITOR
} ScreenFlag;

void UpdateDrawingState(Board *board, float size);

void DrawBoardGrid(Board *board, float SQUARE_SIZE);

void LoadFile(Board *board, int screenHeight, ScreenFlag *screen, float *SQUARE_SIZE, char **pDirent, int i);

void CheckPiecePlayed(Board *board, float SQUARE_SIZE, int clicked, Vector2 mouse);

void CheckButtonPressed(Menu *menu, Board *board, ScreenFlag *screen, Vector2 mouse);

void DrawBackground(Board *board, Menu menu, float SQUARE_SIZE, int screenWidth, int screenHeight, ScreenFlag *screen,
                    Vector2 mouse, int clicked);

void ShowFileSaver(Board *board, int screenWidth, int screenHeight, char *filename, int frameCounter, Vector2 mouse,
                   int clicked, ScreenFlag *screen);

int main() {
    Board board;

    const int screenWidth = 1000;
    const int screenHeight = 800;

    ScreenFlag *screen = malloc(sizeof(ScreenFlag));

    *screen = GAME;

    char filename[11] = "";
    int numOfChars = 0;
    int frameCounter = 0;

    initializeGame(&board, 8, HARD);
    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "Reversi");

    float SQUARE_SIZE = (float) screenHeight / (float) board.size;

    Rectangle goBackButton = (Rectangle) {board.size * SQUARE_SIZE + 20, 30,
                                          screenWidth - board.size * SQUARE_SIZE - 40, 75};

    Rectangle goFowardButton = (Rectangle) {goBackButton.x, goBackButton.height + goBackButton.y + 10,
                                            screenWidth - board.size * SQUARE_SIZE - 40, 75};
    Rectangle saveGameButton = (Rectangle) {goFowardButton.x, goFowardButton.height + goFowardButton.y + 10,
                                            screenWidth - board.size * SQUARE_SIZE - 40, 75};
    Rectangle loadGameButton = (Rectangle) {saveGameButton.x, saveGameButton.height + saveGameButton.y + 10,
                                            screenWidth - board.size * SQUARE_SIZE - 40, 75};

    Menu menu = (Menu) {goBackButton, goFowardButton, saveGameButton, loadGameButton};

    DIR *d;
    struct dirent *dir;
    d = opendir("saved/");

    int count = 0;
    char **dirs = malloc(sizeof(char *));

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char *tmp = calloc(dir->d_namlen + 1, sizeof(char));
            for (int i = 0; i < dir->d_namlen; i++) {
                tmp[i] = dir->d_name[i];
            }
            tmp[dir->d_namlen] = '\0';
            if (IsFileExtension(tmp, ".txt")) {
                count++;
                dirs = realloc(dirs, count * sizeof(char *));
                dirs[count - 1] = tmp;
            }
        }
        closedir(d);
    }


    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        frameCounter = (frameCounter + 1) % 60;
        Vector2 mouse = GetMousePosition();

        int clicked = 0;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) clicked = 1;


        int key = GetKeyPressed();

        while (key > 0) {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && numOfChars < 10) {
                numOfChars++;
                filename[numOfChars - 1] = (char) key;
            }
            // Check next character in the queue


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
                DrawBackground(&board, menu, SQUARE_SIZE, screenWidth, screenHeight, screen, mouse, clicked);
                DrawFPS(10, 10);
                break;
            case SAVE:
                ShowFileSaver(&board, screenWidth, screenHeight, filename, frameCounter, mouse, clicked, screen);
                break;
            case LOAD:
                LoadFile(&board, screenHeight, screen, &SQUARE_SIZE, dirs, count);
                break;
            case EDITOR:
                break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();
    free(filename);
    free(screen);
    return 0;
}

void CheckButtonPressed(Menu *menu, Board *board, ScreenFlag *screen, Vector2 mouse) {
    if (CheckCollisionPointRec(mouse, menu->goBackButton)) {
        goBack(board);
        SetHelpers(board);
    } else if (CheckCollisionPointRec(mouse, menu->goFowardButton)) {
        goForward(board);
    } else if (CheckCollisionPointRec(mouse, menu->saveGameButton)) {
        *screen = SAVE;
    } else if (CheckCollisionPointRec(mouse, menu->loadGameButton)) {
        *screen = LOAD;
    }
}

void CheckPiecePlayed(Board *board, float SQUARE_SIZE, int clicked, Vector2 mouse) {
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            Vector2 vector;
            switch (board->state[i][j].pieceType) {
                case HELPER: {
                    vector = (Vector2) {(i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2};
                    if (CheckCollisionPointCircle(mouse, vector, SQUARE_SIZE / 2 - 5)) {
                        if (!clicked) {
                            DrawRectangle((i) * SQUARE_SIZE + 1, (j) * SQUARE_SIZE + 1, SQUARE_SIZE - 2,
                                          SQUARE_SIZE - 2, DARKGREEN);
                            DrawCircle((i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2,
                                       SQUARE_SIZE / 2 - 5, Fade(BLACK, 0.4f));
                        } else {
                            Movement m = {.pieceType = BLACK_PIECE, .x = i, .y = j};
                            makeRealMove(board, m);
                            removeHistoryFoward(board);
                            computerMove(board);
                        }
                    }
                }
                    break;
                default:
                    break;
            }
        }
    }
}

void DrawBoardGrid(Board *board, float SQUARE_SIZE) {
    for (int i = 0; i < board->size + 1; i++) {
        DrawLineV((Vector2) {SQUARE_SIZE * i, 0}, (Vector2) {SQUARE_SIZE * i, board->size * SQUARE_SIZE}, BLACK);
    }

    for (int i = 0; i < board->size + 1; i++) {
        DrawLineV((Vector2) {0, SQUARE_SIZE * i}, (Vector2) {board->size * SQUARE_SIZE, SQUARE_SIZE * i}, BLACK);
    }
}

void DrawBackground(Board *board, Menu menu, float SQUARE_SIZE, int screenWidth, int screenHeight, ScreenFlag *screen,
                    Vector2 mouse,
                    int clicked) {
    ClearBackground(DARKGREEN);
    DrawBoardGrid(board, SQUARE_SIZE);

    DrawRectangle(board->size * SQUARE_SIZE + 1, 0, screenWidth - 1, screenHeight, WHITE);
    DrawRectangle(board->size * SQUARE_SIZE + 1, 0, screenWidth - 1, screenHeight, Fade(DARKGREEN, 0.5f));
    DrawRectangle(menu.goBackButton.x, menu.goBackButton.y, menu.goBackButton.width, menu.goBackButton.height, WHITE);
    DrawRectangle(menu.goFowardButton.x, menu.goFowardButton.y, menu.goFowardButton.width, menu.goFowardButton.height,
                  WHITE);
    DrawRectangle(menu.saveGameButton.x, menu.saveGameButton.y, menu.saveGameButton.width, menu.saveGameButton.height,
                  WHITE);
    DrawRectangle(menu.loadGameButton.x, menu.loadGameButton.y, menu.loadGameButton.width, menu.loadGameButton.height,
                  WHITE);
    DrawText("Go back", menu.goBackButton.x - MeasureText("Go back", 30) / 2 + menu.goBackButton.width / 2,
             menu.goBackButton.y + menu.goBackButton.height / 2 - 15, 30, canGoBack(board) ? BLACK : GRAY);
    DrawText("Go foward", menu.goFowardButton.x - MeasureText("Go foward", 30) / 2 + menu.goFowardButton.width / 2,
             menu.goFowardButton.y + menu.goFowardButton.height / 2 - 15, 30, canGoFoward(board) ? BLACK : GRAY);
    DrawText("Save game", menu.saveGameButton.x - MeasureText("Save game", 30) / 2 + menu.saveGameButton.width / 2,
             menu.saveGameButton.y + menu.saveGameButton.height / 2 - 15, 30, BLACK);
    DrawText("Load game", menu.loadGameButton.x - MeasureText("Load game", 30) / 2 + menu.loadGameButton.width / 2,
             menu.loadGameButton.y + menu.loadGameButton.height / 2 - 15, 30, BLACK);
    SetHelpers(board);
    UpdateDrawingState(board, SQUARE_SIZE);

    if (clicked)
        CheckButtonPressed(&menu, board, screen, mouse);
    CheckPiecePlayed(board, SQUARE_SIZE, clicked, mouse);

    if (isGameOver(board)) {
        DrawText("Game Over", (SQUARE_SIZE * board->size) / 2 - MeasureText("Game Over", 80) / 2,
                 screenHeight / 2 - 40, 80, GRAY);
        char *text;
        Color color;
        switch (getWinner(board)) {
            case WINNER:
                text = "You win!";
                color = GREEN;
                break;
            case LOSER:
                text = "You lose!";
                color = RED;
                break;
            case TIE:
                text = "It's a tie!";
                color = GRAY;
                break;
        }
        DrawText(text, (SQUARE_SIZE * board->size) / 2 - MeasureText(text, 60) / 2,
                 screenHeight / 2 - 30 + 80 + 10, 60, color);
    }
}

void ShowFileSaver(Board *board, int screenWidth, int screenHeight, char *filename, int frameCounter, Vector2 mouse,
                   int clicked, ScreenFlag *screen) {
    ClearBackground(RAYWHITE);

    int width = fmax(MeasureText(filename, 30), MeasureText("XXXXXXXX", 30)) + 30;

    int saveWidth = MeasureText("Save", 20) + 20;
    int cancelWidth = MeasureText("Cancel", 20) + 20;

    Rectangle saveRect = (Rectangle) {screenWidth / 2 - (saveWidth + cancelWidth + 40) / 2, screenHeight / 2 + 50,
                                      saveWidth, 30};
    Rectangle cancelRect = (Rectangle) {saveRect.x + saveRect.width + 40, saveRect.y, cancelWidth, 30};

    int overSave = CheckCollisionPointRec(mouse, saveRect);
    int overCancel = CheckCollisionPointRec(mouse, cancelRect);

    DrawRectangleRec(saveRect, overSave ? LIGHTGRAY : GRAY);
    DrawText("Save", saveRect.x + 10, saveRect.y + 5, 20, WHITE);
    DrawRectangleRec(cancelRect, overCancel ? LIGHTGRAY : GRAY);
    DrawText("Cancel", cancelRect.x + 10, saveRect.y + 5, 20, WHITE);

    DrawRectangleLines(screenWidth / 2 - width / 2, screenHeight / 2 - 20, width, 40, BLACK);
    DrawText(filename, screenWidth / 2 - width / 2 + 10, screenHeight / 2 - 15, 30, BLACK);
    if (((frameCounter / 20) % 2) == 0)
        DrawText("_", screenWidth / 2 - width / 2 + MeasureText(filename, 30) + 10, screenHeight / 2 - 15, 30, BLACK);

    if (clicked && overSave) {
        mkdir("saved");
        SaveFileText(TextFormat("saved/%s.txt", filename), saveGame(board));
        filename[0] = '\0';
        *screen = GAME;
    }

    if (clicked && overCancel) {
        filename[0] = '\0';
        *screen = GAME;
    }


}

void LoadFile(Board *board, int screenHeight, ScreenFlag *screen, float *SQUARE_SIZE, char **dirs, int count) {
    ClearBackground(RAYWHITE);
    for (int i = 0; i < count; i++) {
        DrawRectangle(0, (i + 1) * 50, MeasureText(dirs[i], 20) + 20, 30, RAYWHITE);
        DrawText(dirs[i], 10, i * 50, 20, BLACK);
    }
}

void UpdateDrawingState(Board *board, float SQUARE_SIZE) {

    int offset = (SQUARE_SIZE / 2 - 5) * 0.25;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            switch (board->state[i][j].pieceType) {
                case VOID:
                    break;
                case BLACK_PIECE:
                    DrawCircle((i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2,
                               SQUARE_SIZE / 2 - 5, BLACK);
                    DrawCircleGradient((i) * SQUARE_SIZE + SQUARE_SIZE / 2 - offset,
                                       (j) * SQUARE_SIZE + SQUARE_SIZE / 2 - offset,
                                       (SQUARE_SIZE / 2 - 5) * 0.5, Fade(WHITE, 0.15), Fade(WHITE, 0));
                    break;
                case WHITE_PIECE:
                    DrawCircle((i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2,
                               SQUARE_SIZE / 2 - 5, RAYWHITE);
                    DrawCircleGradient((i) * SQUARE_SIZE + SQUARE_SIZE / 2 - offset,
                                       (j) * SQUARE_SIZE + SQUARE_SIZE / 2 - offset,
                                       (SQUARE_SIZE / 2 - 5) * 0.5, Fade(BLACK, 0.15), Fade(BLACK, 0));
                    break;
                case HELPER:
                    DrawCircle((i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2,
                               SQUARE_SIZE / 2 - 5, DARKGRAY);
                    DrawCircle((i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2,
                               SQUARE_SIZE / 2 - 7, DARKGREEN);
                    break;
            }
        }
    }
}