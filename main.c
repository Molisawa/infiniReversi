#include <stdio.h>
#include "reversi.c"
#include "raylib.h"

void UpdateDrawingState(Board *board, int size);

void CheckPiecePlayed(Board *board, int SQUARE_SIZE, int clicked);

void CheckButtonPressed(int clicked, Rectangle rectangle, Board *board);

int main()
{
    struct board_struct board;
    printf("BIENVENIDO A REVERSI\n");

    const int screenWidth = 1000;
    const int screenHeight = 800;

    initializeGame(&board);
    InitWindow(screenWidth, screenHeight, "Reversi");

    static Vector2 offset = { 0 };

    int SQUARE_SIZE = screenHeight/board.size;

    offset.x = screenWidth%SQUARE_SIZE;
    offset.y = screenHeight%SQUARE_SIZE;


    Rectangle goBackButton = (Rectangle){board.size * SQUARE_SIZE+20, 30, screenWidth-board.size * SQUARE_SIZE-40,150};


    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(DARKGREEN);
        for (int i = 0; i < board.size + 1; i++)
        {
            DrawLineV((Vector2){SQUARE_SIZE*i + offset.x/2, offset.y/2}, (Vector2){SQUARE_SIZE*i + offset.x/2, board.size * SQUARE_SIZE}, BLACK);
        }

        for (int i = 0; i < board.size + 1; i++)
        {
            DrawLineV((Vector2){offset.x/2, SQUARE_SIZE*i + offset.y/2}, (Vector2){board.size * SQUARE_SIZE, SQUARE_SIZE * i + offset.y / 2}, BLACK);
        }
        DrawRectangle(board.size * SQUARE_SIZE+1, 0,screenWidth-1, screenHeight, WHITE);
        DrawRectangle(board.size * SQUARE_SIZE+1, 0,screenWidth-1, screenHeight, Fade(DARKGREEN, 0.5f));
        DrawRectangle(goBackButton.x, goBackButton.y, goBackButton.width, goBackButton.height, WHITE);
        DrawText("Go back",goBackButton.x-MeasureText("Go back",30)/2+goBackButton.width/2,goBackButton.y+goBackButton.height/2-15,30, BLACK);
        SetHelpers(&board);
        UpdateDrawingState(&board, SQUARE_SIZE);

        int clicked = 0;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            clicked = 1;
        CheckPiecePlayed(&board, SQUARE_SIZE, clicked);
        CheckButtonPressed(clicked, goBackButton, &board);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }



    CloseWindow();

    return 0;
}

void CheckButtonPressed(int clicked, Rectangle rectangle, Board *board) {
    if(clicked && CheckCollisionPointRec(GetMousePosition(),rectangle)) {
        goBack(board);
    }

}

void CheckPiecePlayed(Board *board, int SQUARE_SIZE, int clicked) {
    Vector2 mousePoint = GetMousePosition();
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            Vector2 vector;
            switch (board->state[i][j]->pieceType) {
                case HELPER:
                {
                    vector = (Vector2) {(i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2};
                    if (CheckCollisionPointCircle(mousePoint, vector, SQUARE_SIZE / 2 - 5)) {
                        if (!clicked) {
                            DrawRectangle((i) * SQUARE_SIZE + 1, (j) * SQUARE_SIZE + 1, SQUARE_SIZE - 2,
                                          SQUARE_SIZE - 2, DARKGREEN);
                            DrawCircle((i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2,
                                       SQUARE_SIZE / 2 - 5, Fade(BLACK, 0.4f));
                        } else {
                            Movement m = {.pieceState = BLACK_PIECE, .x = i, .y = j};
                            make_move(board,m);
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

void UpdateDrawingState(Board *board, int SQUARE_SIZE) {

    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            switch (board->state[i][j]->pieceType) {
                case VOID:
                    break;
                case BLACK_PIECE:
                    DrawCircle((i)*SQUARE_SIZE+SQUARE_SIZE/2,(j)*SQUARE_SIZE+SQUARE_SIZE/2,SQUARE_SIZE/2-5,BLACK);
                    break;
                case WHITE_PIECE:
                    DrawCircle((i)*SQUARE_SIZE+SQUARE_SIZE/2,(j)*SQUARE_SIZE+SQUARE_SIZE/2,SQUARE_SIZE/2-5,WHITE);
                    break;
                case HELPER:
                    DrawCircle((i)*SQUARE_SIZE+SQUARE_SIZE/2,(j)*SQUARE_SIZE+SQUARE_SIZE/2,SQUARE_SIZE/2-5,DARKGRAY);
                    DrawCircle((i)*SQUARE_SIZE+SQUARE_SIZE/2,(j)*SQUARE_SIZE+SQUARE_SIZE/2,SQUARE_SIZE/2-7,DARKGREEN);
                    break;
            }
        }
    }
}