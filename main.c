#include <stdio.h>
#include "reversi.c"
#include "raylib.h"

void UpdateDrawingState(struct board_struct *pStruct, int size);

void CheckHumanInteraction(Board *board, int SQUARE_SIZE);

int main()
{
    struct board_struct board;
    printf("BIENVENIDO A REVERSI\n");

    const int screenWidth = 1000;
    const int screenHeight = 800;

    initializeGame(&board);
    InitWindow(screenWidth, screenHeight, "Reversi");

    static Vector2 offset = { 0 };

    int SQUARE_SIZE = screenHeight/BOARD_SIZE;

    printf("%d",SQUARE_SIZE);
    offset.x = screenWidth%SQUARE_SIZE;
    offset.y = screenHeight%SQUARE_SIZE;


    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(DARKGREEN);
        for (int i = 0; i < BOARD_SIZE + 1; i++)
        {
            DrawLineV((Vector2){SQUARE_SIZE*i + offset.x/2, offset.y/2}, (Vector2){SQUARE_SIZE*i + offset.x/2, BOARD_SIZE * SQUARE_SIZE}, BLACK);
        }

        for (int i = 0; i < BOARD_SIZE + 1; i++)
        {
            DrawLineV((Vector2){offset.x/2, SQUARE_SIZE*i + offset.y/2}, (Vector2){BOARD_SIZE * SQUARE_SIZE, SQUARE_SIZE*i + offset.y/2}, BLACK);
        }
        UpdateDrawingState(&board, SQUARE_SIZE);

        CheckHumanInteraction(&board, SQUARE_SIZE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }



    CloseWindow();

    return 0;
}

void CheckHumanInteraction(Board *board,int SQUARE_SIZE) {

    Vector2 mousePoint = GetMousePosition();
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Vector2 vector;
            switch (board->state[i][j]->pieceType) {
                case HELPER:
                    vector = (Vector2) {(i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2};
                    if (CheckCollisionPointCircle(mousePoint, vector, SQUARE_SIZE / 2 - 5)) {
                        DrawRectangle((i) * SQUARE_SIZE+1,(j) * SQUARE_SIZE+1,SQUARE_SIZE-2, SQUARE_SIZE-2, DARKGREEN);
                        DrawCircle((i) * SQUARE_SIZE + SQUARE_SIZE / 2, (j) * SQUARE_SIZE + SQUARE_SIZE / 2,
                                   SQUARE_SIZE / 2 - 5, Fade(BLACK, 0.4f));
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void UpdateDrawingState(Board *board, int SQUARE_SIZE) {

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
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
                    DrawCircle((i)*SQUARE_SIZE+SQUARE_SIZE/2,(j)*SQUARE_SIZE+SQUARE_SIZE/2,SQUARE_SIZE/2-5,BLACK);
                    DrawCircle((i)*SQUARE_SIZE+SQUARE_SIZE/2,(j)*SQUARE_SIZE+SQUARE_SIZE/2,SQUARE_SIZE/2-7,DARKGREEN);
                    break;
            }
        }
    }
}