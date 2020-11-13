#include <stdio.h>
#include "reversi.c"
#include "raylib.h"

int main()
{
    struct board_struct board;
    printf("BIENVENIDO A REVERSI\n");

    initializeGame(&board);

    while (1) {
        display(&board);
        Movement m = askMove();
        make_move(&board, m);
    }


    return 0;
}
