#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reversi.h"

int playReversi()
{
    int SIZE;

    printf("Dime el valor del que querras tu tablero, recuerda que debe ser un numero par: \n");
    scanf("%d", &SIZE);
    if( (SIZE%2) != 0)
    {
        printf("Por favor ingresa un valor par\n\n");
        playReversi();
    }
    return SIZE;
}

Movement askMove() {
    char col;
    int row;
    puts("Dame la posicion a donde te quieres mover");
    scanf("%c%d", &col, &row);
    Movement m = {.pieceState = WHITE_PIECE, .x =col - 'a', .y = row - 1};
    return m;
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
            Board->state[i][j]->pieceType = VOID;
        }
    }
    Board->state[SIZE/2 - 1][SIZE/2 - 1]->pieceType = WHITE_PIECE;
    Board->state[SIZE/2][SIZE/2]->pieceType = WHITE_PIECE;
    Board->state[SIZE/2 - 1][SIZE/2]->pieceType = BLACK_PIECE;
    Board->state[SIZE/2][SIZE/2 - 1]->pieceType = BLACK_PIECE;

}

int isValidMove(Board currentState, int player){

}



void make_move(Board* board, Movement lastMove){
    int SIZE = board->Size;
    int rowchange = 0;
    int colchange = 0;
    int x = 0;
    int y = 0;

    char opponent = (lastMove.pieceState == WHITE_PIECE) ? BLACK_PIECE : WHITE_PIECE;

    board->state[lastMove.x][lastMove.y]->pieceType = lastMove.pieceState;

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

                    if(board->state[x][y]->pieceType == lastMove.pieceState)
                    {
                        while(board->state[x-=rowchange][y-=colchange]->pieceType==opponent)
                            board->state[x][y]->pieceType = lastMove.pieceState;
                        break;
                    }
                }
            }
        }
}





