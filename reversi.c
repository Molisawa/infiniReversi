#include <stdlib.h>
#include <stdio.h>
#include "reversi.h"

/**
 * Initialize the game
 * @param board Receives a Board type structure
 */
void initializeGame(Board *board) {
    board->compScore = 0;
    board->userScore = 0;
    board->noOfMovesBack = 0;
    board->historyBack = malloc(sizeof(Movement));
    board->noOfMovesFoward = 0;
    board->historyForward = malloc(sizeof(Movement));
    board->size = BOARD_SIZE;
    initializeBoard(board);
}
/**
 * Make a computer move
 * @param board Receives a Board type structure
 */
void computerMove(Board *board) {
    makeRealMove(board, bestMove(board));
}
/**
 * Initialize a board
 * @param board Receives a Board type structure
 */
void initializeBoard(Board *board) {

    int SIZE = board->size;
    board->lastPiecetypeMoved = WHITE_PIECE;

    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            board->state[i][j] = malloc(sizeof(Piece));
            board->state[i][j]->pieceType = VOID;
        }
    }
    board->state[SIZE / 2 - 1][SIZE / 2 - 1]->pieceType = WHITE_PIECE;
    board->state[SIZE / 2][SIZE / 2]->pieceType = WHITE_PIECE;
    board->state[SIZE / 2 - 1][SIZE / 2]->pieceType = BLACK_PIECE;
    board->state[SIZE / 2][SIZE / 2 - 1]->pieceType = BLACK_PIECE;

}
/**
 * Check if the game is finished
 * @param board Receives a Board type structure
 * @return The result of the function ´canMove()´
 */
int isGameOver(Board *board) {
    return !canMove(board, BLACK_PIECE) && !canMove(board, WHITE_PIECE);
}
/**
 * Determine who was the winner or if it was a tie.
 * @param board Receives a Board type structure
 * @return If it was a winner, a loser or a tie
 */
int getWinner(Board *board) {
    int white_moves = 0, black_moves = 0;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j]->pieceType == WHITE_PIECE) white_moves++;
            if (board->state[i][j]->pieceType == BLACK_PIECE) black_moves++;
        }
    }
    if (white_moves == black_moves) return TIE;
    if (white_moves > black_moves) {
        return LOSER;
    } else {
        return WINNER;
    }
}
/**
 * Determine if I can go back a move
 * @param board Receives a Board type structure
 * @return The number of moves I can go back
 */
int canGoBack(Board *board) {
    return board->noOfMovesBack > 0;
}
/**
 * Determine if I can go foward a move
 * @param board Receives a Board type structure
 * @return The number of moves I can go foward
 */
int canGoFoward(Board *board) {
    return board->noOfMovesFoward > 0;
}
/**
 * Determine the possible moves that the player can make
 * @param board Receives a Board type structure
 */
void SetHelpers(Board *board) {
    int possibleMoves = 0;
    if (board->lastPiecetypeMoved == BLACK_PIECE && canMove(board, WHITE_PIECE)) return;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j]->pieceType == VOID) {
                Movement m = {.pieceType = BLACK_PIECE, .x= i, .y=j};
                if (isValidMove(board, m)) {
                    possibleMoves++;
                    board->state[i][j]->pieceType = HELPER;
                }
            }
        }
    }
}
/**
 * Get the game score
 * @param board Receives a Board type structure
 * @param piece Receives an integer Pice
 * @return The game score
 */
int getScore(Board *board, int piece) {
    int score = 0;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j]->pieceType == piece)
                score++;
        }
    }
    return score;
}
/**
 * Evaluate the best possible movement for the CPU
 * @param board Receives a Board type structure
 * @return A structure with the coordinates of the best movement
 */
Movement bestMove(Board *board) {
    int bestScore = 0, x, y;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j]->pieceType == HELPER) {
                board->state[i][j]->pieceType = VOID;
            }
        }
    }
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            Board tmp;
            initializeGame(&tmp);
            for (int k = 0; k < board->size; k++) {
                for (int l = 0; l < board->size; l++) {
                    tmp.state[k][l]->pieceType = board->state[k][l]->pieceType;
                }
            }
            if (tmp.state[i][j]->pieceType == VOID) {
                Movement m = {.pieceType = WHITE_PIECE, .x= i, .y=j};
                if (isValidMove(&tmp, m)) {
                    makeMove(&tmp, m);
                    if (getScore(&tmp, WHITE_PIECE) - getScore(board, WHITE_PIECE) > bestScore) {
                        bestScore = getScore(&tmp, WHITE_PIECE) - getScore(board, WHITE_PIECE);
                        x = i;
                        y = j;
                    }
                }
            }
        }
    }
    Movement m = {.pieceType = WHITE_PIECE, .x= x, .y=y};

    return m;
}
/**
 * Evaluate if current movement is valid
 * @param board Receives a Board type structure
 * @param lastMove a Movement type structure
 * @return 1 if it is a valid move, 0 when all hopes fade away
 */
int isValidMove(Board *board, Movement lastMove) {
    char opponent = (lastMove.pieceType == WHITE_PIECE) ? BLACK_PIECE : WHITE_PIECE;

    int colIndex, rowIndex, offset;

    colIndex = lastMove.x - 1;
    rowIndex = lastMove.y;
    offset = 0;
    while (colIndex > 0 && board->state[colIndex][rowIndex]->pieceType == opponent) {
        colIndex--;
        offset++;
    }
    if (colIndex >= 0 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceType && offset > 0) return 1;


    //move down
    colIndex = lastMove.x + 1;
    rowIndex = lastMove.y;
    offset = 0;
    while (colIndex < board->size - 1 && board->state[colIndex][rowIndex]->pieceType == opponent) {
        colIndex++;
        offset++;
    }
    if (colIndex <= board->size - 1 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceType &&
        offset > 0)
        return 1;

    //move left
    colIndex = lastMove.x;
    rowIndex = lastMove.y - 1;
    offset = 0;
    while (rowIndex > 0 && board->state[colIndex][rowIndex]->pieceType == opponent) {
        rowIndex--;
        offset++;
    }
    if (rowIndex >= 0 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceType && offset > 0) return 1;

    //move right
    colIndex = lastMove.x;
    rowIndex = lastMove.y + 1;
    offset = 0;
    while (rowIndex < board->size - 1 && board->state[colIndex][rowIndex]->pieceType == opponent) {
        rowIndex++;
        offset++;
    }
    if (rowIndex <= board->size - 1 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceType &&
        offset > 0)
        return 1;

    //move up left
    colIndex = lastMove.x - 1;
    rowIndex = lastMove.y - 1;
    offset = 0;
    while (colIndex > 0 && rowIndex > 0 && board->state[colIndex][rowIndex]->pieceType == opponent) {
        colIndex--;
        rowIndex--;
        offset++;
    }
    if (colIndex >= 0 && rowIndex >= 0 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceType &&
        offset > 0)
        return 1;

    //move up right
    colIndex = lastMove.x - 1;
    rowIndex = lastMove.y + 1;
    offset = 0;
    while (colIndex > 0 && rowIndex < board->size - 1 && board->state[colIndex][rowIndex]->pieceType == opponent) {
        colIndex--;
        rowIndex++;
        offset++;
    }
    if (colIndex >= 0 && rowIndex <= board->size - 1 &&
        board->state[colIndex][rowIndex]->pieceType == lastMove.pieceType && offset > 0)
        return 1;

    //move down left
    colIndex = lastMove.x + 1;
    rowIndex = lastMove.y - 1;
    offset = 0;
    while (colIndex < board->size - 1 && rowIndex > 0 && board->state[colIndex][rowIndex]->pieceType == opponent) {
        colIndex++;
        rowIndex--;
        offset++;
    }
    if (colIndex <= board->size - 1 && rowIndex >= 0 &&
        board->state[colIndex][rowIndex]->pieceType == lastMove.pieceType && offset > 0)
        return 1;

    //move down right
    colIndex = lastMove.x + 1;
    rowIndex = lastMove.y + 1;
    offset = 0;
    while (colIndex < board->size - 1 && rowIndex < board->size - 1 &&
           board->state[colIndex][rowIndex]->pieceType == opponent) {
        colIndex++;
        rowIndex++;
        offset++;
    }
    if (colIndex <= board->size - 1 && rowIndex <= board->size - 1 &&
        board->state[colIndex][rowIndex]->pieceType == lastMove.pieceType && offset > 0)
        return 1;

    //when all hopes fade away
    return 0;
}
/**
 * Determine if I can make a move
 * @param board Receives a Board type structure
 * @param Piece Receives an integer Pice
 * @return 1 if I can make a move, 0 when all hopes fade away
 */
int canMove(Board *board, int Piece) {
    int moves = 0;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j]->pieceType == VOID || board->state[i][j]->pieceType == HELPER) {
                Movement m_black = {.pieceType = Piece, .x= i, .y=j};
                if (isValidMove(board, m_black))
                    moves++;
            }
        }
    }
    if (moves == 0) return 0;
    return 1;
}
/**
 * Go back one movement
 * @param board Receives a Board type structure
 */
void goBack(Board *board) {
    if (canGoBack(board)) {
        Movement *m = malloc(sizeof(Movement) * (board->noOfMovesBack - 1));
        for (int i = 0; i < board->noOfMovesBack - 1; ++i) {
            m[i] = board->historyBack[i];
        }
        board->historyForward = realloc(board->historyForward, sizeof(Movement) * (board->noOfMovesFoward + 1));
        board->historyForward[board->noOfMovesFoward] = board->historyBack[board->noOfMovesBack - 1];
        board->noOfMovesFoward += 1;
        int moves = board->noOfMovesBack;
        initializeBoard(board);
        board->historyBack = realloc(board->historyBack, sizeof(Movement) * (moves - 1));
        board->noOfMovesBack = 0;
        for (int i = 0; i < moves - 1; ++i) {
            makeRealMove(board, m[i]);
        }
    }
}
/**
 * Go foward one movement
 * @param board Receives a Board type structure
 */
void goForward(Board *board) {
    if (canGoFoward(board)) {
        Movement *m = malloc(sizeof(Movement) * (board->noOfMovesFoward - 1));
        for (int i = 0; i < board->noOfMovesFoward - 1; ++i) {
            m[i] = board->historyForward[i];
        }
        board->historyBack = realloc(board->historyBack, sizeof(Movement) * (board->noOfMovesBack + 1));
        board->historyBack[board->noOfMovesBack] = board->historyForward[board->noOfMovesFoward - 1];
        int movesForward = --board->noOfMovesFoward;
        int moves = board->noOfMovesBack + 1;
        Movement *historyRebuild = malloc(sizeof(Movement) * moves);
        for (int i = 0; i < moves; i++) {
            historyRebuild[i] = board->historyBack[i];
        }
        initializeBoard(board);
        board->historyForward = realloc(board->historyForward, sizeof(Movement) * (movesForward));
        board->historyForward = m;
        board->noOfMovesBack = 0;
        for (int i = 0; i < moves; ++i) {
            makeRealMove(board, historyRebuild[i]);
        }
    }
}
/**
 * Delete history of forward movements
 * @param board Receives a Board type structure
 */
void removeHistoryFoward(Board *board) {
    free(board->historyForward);
    board->historyForward = malloc(sizeof(Movement));
    board->noOfMovesFoward = 0;
}

/**
 * Make a real move on the board
 * @param board Receives a Board type structure
 * @param lastMove
 */
void makeRealMove(Board *board, Movement lastMove) {
    board->lastPiecetypeMoved = lastMove.pieceType;
    if (lastMove.x < 0 || lastMove.x > board->size - 1 || lastMove.y < 0 || lastMove.y > board->size - 1)return;
    board->historyBack = realloc(board->historyBack, sizeof(Movement) * (board->noOfMovesBack + 1));
    board->historyBack[board->noOfMovesBack] = lastMove;
    board->noOfMovesBack++;
    makeMove(board, lastMove);
}

/**
 * Make a temporary move on the board
 * @param board Receives a Board type structure
 * @param lastMove a Movement type structure
 */
void makeMove(Board *board, Movement lastMove) {
    char opponent = (lastMove.pieceType == WHITE_PIECE) ? BLACK_PIECE : WHITE_PIECE;

    int rowIndex, colIndex;
    Board tmp;
    initializeGame(&tmp);
    for (int k = 0; k < board->size; k++) {
        for (int l = 0; l < board->size; l++) {
            tmp.state[k][l]->pieceType = board->state[k][l]->pieceType;
        }
    }


    tmp.state[lastMove.x][lastMove.y]->pieceType = lastMove.pieceType;

    //move down
    Movement *moves;
    moves = malloc(sizeof(Movement));
    rowIndex = lastMove.x - 1;
    colIndex = lastMove.y;
    int counter = 0;
    while (rowIndex > 0 && board->state[rowIndex][colIndex]->pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex--;
    }
    if (rowIndex >= 0 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceType && counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceType;
        }
    }


    //move up
    calloc(1, sizeof(Movement));
    rowIndex = lastMove.x + 1;
    colIndex = lastMove.y;
    counter = 0;
    while (rowIndex < board->size - 1 && board->state[rowIndex][colIndex]->pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex++;
    }
    if (rowIndex <= board->size - 1 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceType &&
        counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceType;
        }
    }

    //move right
    calloc(1, sizeof(Movement));
    rowIndex = lastMove.x;
    colIndex = lastMove.y - 1;
    counter = 0;
    while (colIndex > 0 && board->state[rowIndex][colIndex]->pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        colIndex--;
    }
    if (colIndex >= 0 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceType && counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceType;
        }
    }


    //move left
    calloc(1, sizeof(Movement));
    rowIndex = lastMove.x;
    colIndex = lastMove.y + 1;
    counter = 0;
    while (colIndex < board->size - 1 && board->state[rowIndex][colIndex]->pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        colIndex++;
    }
    if (colIndex <= board->size - 1 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceType &&
        counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceType;
        }
    }

    //move down right
    calloc(1, sizeof(Movement));
    rowIndex = lastMove.x - 1;
    colIndex = lastMove.y - 1;
    counter = 0;
    while (rowIndex > 0 && colIndex > 0 && board->state[rowIndex][colIndex]->pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex--;
        colIndex--;
    }
    if (rowIndex >= 0 && colIndex >= 0 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceType &&
        counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceType;
        }
    }

    //move up right
    calloc(1, sizeof(Movement));
    rowIndex = lastMove.x - 1;
    colIndex = lastMove.y + 1;
    counter = 0;
    while (rowIndex > 0 && colIndex < board->size - 1 && board->state[rowIndex][colIndex]->pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex--;
        colIndex++;
    }
    if (rowIndex >= 0 && colIndex <= board->size - 1 &&
        board->state[rowIndex][colIndex]->pieceType == lastMove.pieceType && counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceType;
        }
    }

    //move down left
    calloc(1, sizeof(Movement));
    rowIndex = lastMove.x + 1;
    colIndex = lastMove.y - 1;
    counter = 0;
    while (rowIndex < board->size - 1 && colIndex > 0 && board->state[rowIndex][colIndex]->pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex++;
        colIndex--;
    }
    if (rowIndex <= board->size - 1 && colIndex >= 0 &&
        board->state[rowIndex][colIndex]->pieceType == lastMove.pieceType && counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceType;
        }
    }

    //move up left
    calloc(1, sizeof(Movement));
    rowIndex = lastMove.x + 1;
    colIndex = lastMove.y + 1;
    counter = 0;
    while (rowIndex < board->size - 1 && colIndex < board->size - 1 &&
           board->state[rowIndex][colIndex]->pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex++;
        colIndex++;
    }
    if (rowIndex <= board->size - 1 && colIndex <= board->size - 1 &&
        board->state[rowIndex][colIndex]->pieceType == lastMove.pieceType && counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceType;
        }
    }


    for (int k = 0; k < board->size; k++) {
        for (int l = 0; l < board->size; l++) {
            board->state[k][l]->pieceType = tmp.state[k][l]->pieceType;
        }
    }
}


/**
 * Save current game
 * @param board Receives a Board type structure
 */
void saveGame(Board *board){
    FILE *outfile;
    outfile = fopen ("gameState.txt", "wb");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
    fwrite(&board->state,sizeof(Board),1,outfile);
    // close file
    fclose (outfile);
}




