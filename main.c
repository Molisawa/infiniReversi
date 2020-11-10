#include <stdio.h>
#include "reversi.c"

int main()
{
    struct board_struct board;
    printf("BIENVENIDO A REVERSI\n");

    initializeGame(&board);
    display(&board);

    return 0;
}
