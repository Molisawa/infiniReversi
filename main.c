#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-integer-division"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include <stdio.h>
#include "reversi.c"
#include "raylib.h"


typedef struct menu {
    Rectangle goBackButton, goFowardButton;
}Menu;


void UpdateDrawingState(Board *board, int size);

void CheckPiecePlayed(Board *board, int SQUARE_SIZE, int clicked);

void CheckButtonPressed(Menu *menu,Board *board);

int main()
{
    Board board;

    const int screenWidth = 1000;
    const int screenHeight = 800;


    initializeGame(&board);
    InitWindow(screenWidth, screenHeight, "Reversi");

    static Vector2 offset = { 0 };

    int SQUARE_SIZE = screenHeight/board.size;

    offset.x = screenWidth%SQUARE_SIZE;
    offset.y = screenHeight%SQUARE_SIZE;


    Rectangle goBackButton = (Rectangle){board.size * SQUARE_SIZE+20, 30, screenWidth-board.size * SQUARE_SIZE-40,150};

    Rectangle goFowardButton = (Rectangle){goBackButton.x, goBackButton.height + goBackButton.y + 10, screenWidth - board.size * SQUARE_SIZE - 40, 150};

    Menu menu = (Menu){goBackButton, goFowardButton};

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
        DrawRectangle(goFowardButton.x, goFowardButton.y, goFowardButton.width, goFowardButton.height, WHITE);
        DrawText("Go back",goBackButton.x-MeasureText("Go back",30)/2+goBackButton.width/2,goBackButton.y+goBackButton.height/2-15,30, canGoBack(&board)? BLACK: GRAY);
        DrawText("Go foward",goFowardButton.x-MeasureText("Go foward",30)/2+goFowardButton.width/2,goFowardButton.y+goFowardButton.height/2-15,30, canGoFoward(&board)? BLACK: GRAY);

        DrawText("Your score:",goBackButton.x, goFowardButton.height+goFowardButton.y+30, 20, WHITE);
        int val = getScore(&board, BLACK_PIECE);
        char *scoreWhite = malloc(sizeof(char)*val);
        itoa(val,scoreWhite,10);
        DrawText(scoreWhite,goBackButton.x, goFowardButton.height+goFowardButton.y+50, 20, WHITE);

        DrawText("CPU score:",goBackButton.x, goFowardButton.height+goFowardButton.y+100, 20, WHITE);
        val = getScore(&board, WHITE_PIECE);
        *scoreWhite = realloc(scoreWhite,sizeof(char)*val);
        itoa(val,scoreWhite,10);
        DrawText(scoreWhite,goBackButton.x, goFowardButton.height+goFowardButton.y+120, 20, WHITE);

        SetHelpers(&board);
        UpdateDrawingState(&board, SQUARE_SIZE);

        int clicked = 0;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            clicked = 1;
            CheckButtonPressed(&menu, &board);
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
                    color =  GREEN;
                    break;
                case LOSER:
                    text = "You lose!";
                    color= RED;
                    break;
                case TIE:
                    text =  "It's a tie!";
                    color=GRAY;
                    break;
            }
            DrawText(text, (SQUARE_SIZE * board.size) / 2 - MeasureText(text, 60) / 2,
                     screenHeight / 2 - 30+80+10, 60, color);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }



    CloseWindow();

    return 0;
}

void CheckButtonPressed(Menu *menu, Board *board) {
    if(CheckCollisionPointRec(GetMousePosition(),menu->goBackButton)) {
        goBack(board);
        SetHelpers(board);
    } else if(CheckCollisionPointRec(GetMousePosition(),menu->goFowardButton)) {
        goForward(board);
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
                            makeMove(board, m);
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