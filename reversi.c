#include <stdlib.h>
#include "reversi.h"


void initializeGame(Board *board){
    board->compScore = 0;
    board->userScore = 0;
    board->noOfMoves = malloc(sizeof(int));
    board->noOfMoves[0] = 0;
    board->history = malloc(sizeof(Movement));
    board->size = BOARD_SIZE;
    initializeBoard(board);
}

void initializeBoard(Board *board){

    int SIZE = board->size;

    for(int i=0;i< board->size; i++)
    {
        for(int j=0;j< board->size; j++)
        {
            board->state[i][j] = malloc(sizeof(Piece));
            board->state[i][j]->pieceType = VOID;
        }
    }
    board->state[SIZE/2 - 1][SIZE/2 - 1]->pieceType = WHITE_PIECE;
    board->state[SIZE/2][SIZE/2]->pieceType = WHITE_PIECE;
    board->state[SIZE/2 - 1][SIZE/2]->pieceType = BLACK_PIECE;
    board->state[SIZE/2][SIZE/2 - 1]->pieceType = BLACK_PIECE;

}

int isGameOver(Board *board) {
    int white_moves = 0;
    int black_moves = 0;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j]->pieceType == VOID || board->state[i][j]->pieceType ==HELPER) {
                Movement m_black = {.pieceState = BLACK_PIECE, .x= i, .y=j};
                Movement m_white = {.pieceState = WHITE_PIECE, .x= i, .y=j};
                if (isValidMove(board, m_black))
                    black_moves++;
                if (isValidMove(board, m_white))
                    white_moves++;
            }
        }
    }
    if (white_moves == 0 && black_moves == 0) return 1;

    return 0;
}

void SetHelpers(Board *board){
    for(int i=0;i<board->size; i++){
        for(int j = 0; j < board->size; j++) {
            if(board->state[i][j]->pieceType == VOID) {
                Movement m = {.pieceState = BLACK_PIECE, .x= i, .y=j};
                if (isValidMove(board,m)){
                    board->state[i][j]->pieceType = HELPER;
                }
            }
        }
    }
}

int isValidMove(Board *board, Movement lastMove){
    int rowchange = 0;
    int colchange = 0;
    int x = 0;
    int y = 0;

    char opponent = (lastMove.pieceState == WHITE_PIECE) ? BLACK_PIECE : WHITE_PIECE;


    for(rowchange = -1; rowchange <= 1; rowchange++)
        for(colchange = -1; colchange <= 1; colchange++)
        {
            if(lastMove.x + rowchange < 0 || lastMove.x + rowchange >= BOARD_SIZE ||
               lastMove.y + colchange < 0 || lastMove.y + colchange >= BOARD_SIZE ||
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


                    if(x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
                        return 0;


                    if(board->state[x][y]->pieceType == VOID || board->state[x][y]->pieceType == HELPER)
                        return 0;

                    if(board->state[x][y]->pieceType == lastMove.pieceState)
                        return 1;
                }
            }
        }
    return 0;
}

void goBack(Board *board){
    if (board->noOfMoves[0]>0) {
        Movement *m = malloc(sizeof(Movement) * (board->noOfMoves[0] - 1));
        for (int i = 0; i < board->noOfMoves[0]; ++i) {
            m[i] = board->history[i];
        }
        free(board->history);
        initializeBoard(board);
        board->history = m;
        board->noOfMoves[0]--;
        for(int i =0; i < board->noOfMoves[0];++i){
            make_move(board, m[i]);
        }
    }

}

void make_move(Board *board, Movement lastMove){
    int SIZE = board->size;
    int rowchange = 0;
    int colchange = 0;
    int x = 0;
    int y = 0;

    for(int i = 0; i < board->size; i++){
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j]->pieceType == HELPER) {
                board->state[i][j]->pieceType = VOID;
            }
        }
    }

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


                    if(board->state[x][y]->pieceType == VOID)
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
    board->history = realloc(board->history,sizeof(Movement)*(board->noOfMoves[0]+1));
    board->history[board->noOfMoves[0]] = lastMove;
    board->noOfMoves[0] +=1;
}








