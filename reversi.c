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
        printf("Por favor ingresa un valor par: \n");
        scanf("%d",&SIZE);
    }
    
}

void initializeGame(Board* b,int SIZE){

b->noOfGames = 0;
b->currentPlayer = 0;
b->compScore = 0;
b->noOfGames = 0;
b->invalidMoves = 0;
b->userScore = 0;
initializeBoard(b->state,SIZE);
}

void initializeBoard(Board* Board, int SIZE){

 Board->state[SIZE/2 - 1][SIZE/2 - 1] = Board->state[SIZE/2][SIZE/2] = malloc(sizeof(Piece));
 Board->state[SIZE/2 - 1][SIZE/2] = Board->state[SIZE/2][SIZE/2 - 1] = malloc(sizeof(Piece));
 Board->state[SIZE/2 - 1][SIZE/2 - 1]->pieceType = 'O';
 Board->state[SIZE/2][SIZE/2]->pieceType = 'O';
 Board->state[SIZE/2 - 1][SIZE/2]->pieceType = 'X';
 Board->state[SIZE/2][SIZE/2 - 1]->pieceType = 'X';

}

int isValidMove(Board currentState, int player){

}

int get_score(Board* b,Piece* p)
{
    int score = 0;
    int row = 0;
    int col = 0;

    char opponent = p->player == 'O' ? '@' : 'O';

}
