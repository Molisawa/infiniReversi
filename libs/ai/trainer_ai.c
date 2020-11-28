#include "trainer_ai.h"
#include <io.h>
#include <gsl/gsl_matrix.h>

void train(int numberOfBoards, const int sizeOfBoard) {
    int count = 0;
    gsl_matrix *abber;
    abber = gsl_matrix_alloc(sizeOfBoard,sizeOfBoard);
    for (int i = 0;i<sizeOfBoard;i++){
        for (int j =0; j<sizeOfBoard; j++){
            printf ("%f ", gsl_matrix_get(abber,i,j));
        }
        puts("");
    }
    gsl_matrix_free(abber);
    mkdir("brain");
    while (count < numberOfBoards) {
        Board board;
        initializeGame(&board, sizeOfBoard, INTERMEDIATE, false, (Player) {0}, (Player) {0});
        while (!isGameOver(&board)) {
            makeRealMove(&board, epsilonMove(&board, nextTurn(&board)));
        }
        destructBoard(&board);
        count++;
    }
}

Movement epsilonMove(Board *board, PlayerType player) {
    if (GetRandomValue(0, 9) < 9) {
        return bestMove(board, player);
    } else return randomMovement(board, player);
}