#include <stdio.h>
#include "reversi.c"

int main()
{
    struct board_struct board;
    printf("BIENVENIDO A REVERSI\n");

    initializeGame(&board);
    display(&board);
    //Movement movement = {.pieceType = 'O', .x = 5, .y= 3};
    //make_move(&board,movement);
    //display(&board);
    //Movement movement1 = {.pieceType = 'O', .x = 2, .y= 5};
    //make_move(&board,movement1);
    //display(&board);
    return 0;
}
