#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-integer-division"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include "reversi_game_engine.h"
#include <raylib.h>

typedef struct menu {
    Rectangle goBackButton, goFowardButton, saveGameButton, loadGameButton;
} Menu;

void UpdateDrawingState(Board *board, float size);

void CheckPiecePlayed(Board *board, float SQUARE_SIZE, int clicked);

void CheckButtonPressed(Menu *menu, Board *board, int screenHeight, float *SQUARE_SIZE);

void DrawBackground(Board board, Menu menu, float SQUARE_SIZE, int screenWidth, int screenHeight);

int main() {
    Board board;

    const int screenWidth = 1000;
    const int screenHeight = 800;

    int save = 0;

    char *filename;
    int numOfCars = 0;

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

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(DARKGREEN);


        if (canSkipBlackPiece(&board)) {
            computerMove(&board);
        }

        DrawBackground(board, menu,SQUARE_SIZE, screenWidth, screenHeight);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();

    return 0;
}

void CheckButtonPressed(Menu *menu, Board *board, int screenHeight, float *SQUARE_SIZE) {
    if (CheckCollisionPointRec(GetMousePosition(), menu->goBackButton)) {
        goBack(board);
        SetHelpers(board);
    } else if (CheckCollisionPointRec(GetMousePosition(), menu->goFowardButton)) {
        goForward(board);
    } else if (CheckCollisionPointRec(GetMousePosition(), menu->saveGameButton)) {
        char *game = saveGame(board);
        SaveFileText("game.txt", game);
    } else if (CheckCollisionPointRec(GetMousePosition(), menu->loadGameButton)) {
        Board boardTemp = loadGame(LoadFileText("game.txt"));
        if (boardTemp.initialized != 1)
            return;
        *board = boardTemp;
        *SQUARE_SIZE = (float) screenHeight / (float) board->size;
    }
}

void CheckPiecePlayed(Board *board, float SQUARE_SIZE, int clicked) {
    Vector2 mousePoint = GetMousePosition();
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            Vector2 vector;
            switch (board->state[i][j].pieceType) {
                case HELPER: {
                    vector = (Vector2) {(i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2};
                    if (CheckCollisionPointCircle(mousePoint, vector, SQUARE_SIZE / 2 - 5)) {
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

void DrawBackground(Board board, Menu menu, float SQUARE_SIZE, int screenWidth, int screenHeight){
    for (int i = 0; i < board.size + 1; i++) {
        DrawLineV((Vector2) {SQUARE_SIZE * i, 0}, (Vector2) {SQUARE_SIZE * i, board.size * SQUARE_SIZE}, BLACK);
    }

    for (int i = 0; i < board.size + 1; i++) {
        DrawLineV((Vector2) {0, SQUARE_SIZE * i}, (Vector2) {board.size * SQUARE_SIZE, SQUARE_SIZE * i}, BLACK);
    }
    DrawRectangle(board.size * SQUARE_SIZE + 1, 0, screenWidth - 1, screenHeight, WHITE);
    DrawRectangle(board.size * SQUARE_SIZE + 1, 0, screenWidth - 1, screenHeight, Fade(DARKGREEN, 0.5f));
    DrawRectangle(menu.goBackButton.x, menu.goBackButton.y, menu.goBackButton.width, menu.goBackButton.height, WHITE);
    DrawRectangle(menu.goFowardButton.x, menu.goFowardButton.y, menu.goFowardButton.width, menu.goFowardButton.height, WHITE);
    DrawRectangle(menu.saveGameButton.x, menu.saveGameButton.y, menu.saveGameButton.width, menu.saveGameButton.height, WHITE);
    DrawRectangle(menu.loadGameButton.x, menu.loadGameButton.y, menu.loadGameButton.width, menu.loadGameButton.height, WHITE);
    DrawText("Go back", menu.goBackButton.x - MeasureText("Go back", 30) / 2 + menu.goBackButton.width / 2,
             menu.goBackButton.y + menu.goBackButton.height / 2 - 15, 30, canGoBack(&board) ? BLACK : GRAY);
    DrawText("Go foward", menu.goFowardButton.x - MeasureText("Go foward", 30) / 2 + menu.goFowardButton.width / 2,
             menu.goFowardButton.y + menu.goFowardButton.height / 2 - 15, 30, canGoFoward(&board) ? BLACK : GRAY);
    DrawText("Save game", menu.saveGameButton.x - MeasureText("Save game", 30) / 2 + menu.saveGameButton.width / 2,
             menu.saveGameButton.y + menu.saveGameButton.height / 2 - 15, 30, BLACK);
    DrawText("Load game", menu.loadGameButton.x - MeasureText("Load game", 30) / 2 + menu.loadGameButton.width / 2,
             menu.loadGameButton.y + menu.loadGameButton.height / 2 - 15, 30, BLACK);
    SetHelpers(&board);
    UpdateDrawingState(&board, SQUARE_SIZE);

    int clicked = 0;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        clicked = 1;
        CheckButtonPressed(&menu, &board, screenHeight, &SQUARE_SIZE);
    }
    CheckPiecePlayed(&board, SQUARE_SIZE, clicked);

    if (isGameOver(&board)) {
        DrawText("Game Over", (SQUARE_SIZE * board.size) / 2 - MeasureText("Game Over", 80) / 2,
                 screenHeight / 2 - 40, 80, GRAY);
        char *text;
        Color color;
        switch (getWinner(&board)) {
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
        DrawText(text, (SQUARE_SIZE * board.size) / 2 - MeasureText(text, 60) / 2,
                 screenHeight / 2 - 30 + 80 + 10, 60, color);
    }
}

void UpdateDrawingState(Board *board, float SQUARE_SIZE) {

    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            switch (board->state[i][j].pieceType) {
                case VOID:
                    break;
                case BLACK_PIECE:
                    DrawCircle((i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2,
                               SQUARE_SIZE / 2 - 5, BLACK);
                    break;
                case WHITE_PIECE:
                    DrawCircleGradient((i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2,
                                       SQUARE_SIZE / 2 - 5, LIGHTGRAY, WHITE);
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