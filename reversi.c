#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reversi.h"

void playReversi()
{
    int SIZE;

    printf("Dime el valor del que querras tu tablero, recuerda que debe ser un numero par: \n");
    scanf("%d", &SIZE);
    if( (SIZE%2) != 0)
    {
        printf("Por favor ingresa un valor par\n\n");
        playReversi();
    }

}

void initializeGame(Board* b){

    b->noOfGames = 0;
    b->currentPlayer = 0;
    b->compScore = 0;
    b->noOfGames = 0;
    b->invalidMoves = 0;
    b->userScore = 0;
    b->Size = BOARD_SIZE;
    initializeBoard(b);
}

void initializeBoard(Board* Board){

    int SIZE = Board->Size;

    for(int i=0;i< Board->Size;i++)
    {
        for(int j=0;j< Board->Size;j++)
        {
            Board->state[i][j] = malloc(sizeof(Piece));
            Board->state[i][j]->pieceType = ' ';
        }
    }
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

        for (col = 0; col < SIZE; col++) {
            printf(" %c |", b->state[col][row]->pieceType);

        }
        printf("\n");
    }

    printf("  +");
    for (col = 0; col < SIZE; col++)
        printf("---+");
    printf("\n");
}



void make_move(Board* board, Movement lastMove){
    int SIZE = board->Size;
    int rowchange = 0;
    int colchange = 0;
    int x = 0;
    int y = 0;

    char opponent =(lastMove.pieceType == 'O') ? 'X' : 'O';

    board->state[lastMove.x][lastMove.y]->pieceType = lastMove.pieceType;

    for(rowchange = -1; rowchange <= 1; rowchange++)
        for(colchange = -1; colchange <= 1; colchange++)
        {
            if(lastMove.x + rowchange < 0 || lastMove.x + rowchange >= SIZE ||
               lastMove.y + colchange < 0 || lastMove.y + colchange >= SIZE ||
               (rowchange==0 && colchange== 0))
                continue;

            if(board->state[lastMove.x + rowchange][lastMove.y + colchange]->pieceType == opponent)
            {

                x = lastMove.x + rowchange;
                y = lastMove.y + colchange;

                for(;;)
                {
                    x += rowchange;
                    y += colchange;


                    if(x < 0 || x >= SIZE || y < 0 || y >= SIZE)
                        break;


                    if(board->state[x][y]->pieceType == ' ')
                        break;

                    if(board->state[x][y]->pieceType == lastMove.pieceType)
                    {
                        while(board->state[x-=rowchange][y-=colchange]->pieceType==opponent)
                            board->state[x][y]->pieceType = lastMove.pieceType;
                        break;
                    }
                }
            }
        }
}





