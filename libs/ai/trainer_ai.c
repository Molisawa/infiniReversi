#include "trainer_ai.h"
#include <raylib.h>
#include <io.h>

void train(int numberOfBoards, int sizeOfBoard) {
    int count = 0;
    mkdir("train");
    while (count < numberOfBoards) {
        Board board;
        initializeGame(&board, sizeOfBoard, INTERMEDIATE, false, (Player) {0}, (Player) {0});
        while (!isGameOver(&board)) {
            makeRealMove(&board, epsilonMove(&board, nextTurn(&board)));
        }
        SaveFileText(TextFormat("train/game_%d.brd", count), saveGame(&board));
        destructBoard(&board);
        count++;
    }
}

Movement epsilonMove(Board *board, PlayerType player) {
    if (GetRandomValue(0, 9) < 9) {
        return bestMove(board, player);
    } else return randomMovement(board, player);
}