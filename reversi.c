#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reversi.h"

void playReversi()
{
    int SIZE;

    printf("Dime el valor del que querrás tu tablero, recuerda que debe ser un número par: \n");
    scanf("%d", &SIZE);
    if( (SIZE%2) != 0)
    {
        printf("P-or favor ingresa un valor par: \n");
        scanf("%d",&SIZE);
    }

}

void initializeGame(Board* b){

b->noOfGames = 0;
b->currentPlayer = 0;
b->compScore = 0;
b->noOfGames = 0;
b->invalidMoves = 0;
b->userScore = 0;
b->Size = 8;
initializeBoard(b);
}

void initializeBoard(Board* Board){

    int SIZE = Board->Size;

    for(int i=0;i< Board->Size;i++)
    {
        for(int j=0;j< Board->Size;j++)
        {
            Board->state[i][j] = NULL;
        }
    }

 Board->state[SIZE/2 - 1][SIZE/2 - 1] = Board->state[SIZE/2][SIZE/2] = malloc(sizeof(Piece));
 Board->state[SIZE/2 - 1][SIZE/2] = Board->state[SIZE/2][SIZE/2 - 1] = malloc(sizeof(Piece));
 Board->state[SIZE/2 - 1][SIZE/2 - 1]->pieceType = 'O';
 Board->state[SIZE/2][SIZE/2]->pieceType = 'O';
 Board->state[SIZE/2 - 1][SIZE/2]->pieceType = 'X';
 Board->state[SIZE/2][SIZE/2 - 1]->pieceType = 'X';

}

int isValidMove(Board currentState, int player){

}

void display(Board* b){

    int SIZE = b->Size;

    int row  = 0;
    int col = 0;
    char col_label = 'a';

        printf("\n ");
        for (col = 0; col < SIZE; col++)
            printf("   %c", col_label + col);
        printf("\n");

        for (row = 0; row < SIZE; row++) {
            printf("  +");

            for (col = 0; col < SIZE; col++)
                printf("---+");
            printf("\n%2d|", row + 1);

            for (col = 0; col < SIZE; col++) { //YA IMPRIME EL TABLERO PERO LAS FICHAS DEL CENTRO SE IMPRIMEN MAL
                if (b->state[row][col] != NULL)
                    printf(" %c |", b->state[row][col]->pieceType);
            }
            printf("\n");
        }

        printf("  +");
        for (col = 0; col < SIZE; col++)
            printf("---+");
        printf("\n");
    }
