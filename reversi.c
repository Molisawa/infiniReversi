#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reversi.h"
void initializeGame(Board* b){
b->noOfGames = 0;
b->currentPlayer = 0;
b->compScore = 0;
b->noOfGames = 0;
b->invalidMoves = 0;
b->userScore = 0;
initializeBoard(b->state);
}
void initializeBoard(Board* Board){
 Board->state[8/2 - 1][8/2 - 1] = Board->state[8/2][8/2] = malloc(sizeof(Piece));
 Board->state[8/2 - 1][8/2] = Board->state[8/2][8/2 - 1] = malloc(sizeof(Piece));
 Board->state[8/2 - 1][8/2 - 1]->pieceType = 'O';
 Board->state[8/2][8/2]->pieceType = 'O';
 Board->state[8/2 - 1][8/2]->pieceType = 'X';
 Board->state[8/2][8/2 - 1]->pieceType = 'X';
}
int isValidMove(Board currentState, int player){

}