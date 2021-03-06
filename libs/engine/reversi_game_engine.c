#include <stdlib.h>
#include "reversi_game_engine.h"
#include "../cjson/cJSON.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

int nodes = 0;

/**
 * Initialize the game
 * @param board Receives a Board type structure
 */
void initializeGame(Board *board, int size, int difficulty, bool custom, Player player1, Player player2) {
    board->initialized = 1;
    board->difficulty = difficulty;
    board->noOfMovesBack = 0;
    board->historyBack = malloc(sizeof(Movement));
    board->noOfMovesFoward = 0;
    board->historyForward = malloc(sizeof(Movement));
    board->size = size;
    board->custom = custom;
    board->player1 = player1;
    board->player2 = player2;
    initializeBoard(board);
}

/**
 * Evaluates the movements backward, if bigger than 25, returns the score, otherwise, returns the score of the position.
 * @param board receives a Board type structure
 * @param pieceType receives
 */
int getPointEvaluator(Board *board, int pieceType) {
    return getScorePosition(board, pieceType);
}

/**
 * Gets the score according to the position
 * @param board receives a Board type structure
 * @param pieceType receives an integer pieceType
 * @return the score
 */
int getScorePosition(Board *board, int pieceType) {
    int **valores = (int **) calloc(board->size, sizeof(int *));

    for (int i = 0; i < board->size; i++) {
        valores[i] = calloc(board->size, sizeof(int));
    }

    for (int i = 0; i < board->size; i++) {
        valores[1][i] = -20;
        valores[board->size - 2][i] = -20;
        if (i < board->size - 4) {
            valores[0][i + 2] = 7;
            valores[board->size - 1][i + 2] = 7;
        }
        for (int j = 0; j < board->size; j++) {
            valores[j][1] = -20;
            valores[j][board->size - 2] = -20;
            if (j < board->size - 4) {
                valores[j + 2][0] = 7;
                valores[j + 2][board->size - 1] = 7;
            }
            if (j < board->size - 4 && i < board->size - 4) {
                valores[j + 2][i + 2] = 4;
            }
        }
    }

    for (int i = 1; i <= 2; i++) {
        int value = i % 2;
        int isNegative = value ? -1 : 1;
        int tmp = (100 / (value + 1) * isNegative);
        valores[value][value] = (100 / (value + 1) * isNegative);
        valores[board->size - 1 - value][value] = (100 / (value + 1) * isNegative);
        valores[value][board->size - 1 - value] = (100 / (value + 1) * isNegative);
        valores[board->size - 1 - value][board->size - value - 1] = (100 / (value + 1) * isNegative);
    }

    int score = 0;
    for (int i = 0; i < board->size; ++i) {
        for (int j = 0; j < board->size; ++j) {
            if (board->state[i][j].pieceType == pieceType) {
                score += valores[i][j];
            }
        }
    }
    for (int i = 0; i < board->size; i++) {
        free(valores[i]);
    }
    free(valores);
    return score;
}

/**
 * Make a computer move
 * @param board Receives a Board type structure
 */
void computerMove(Board *board, PlayerType player) {
    srand(time(NULL));
    cleanHelpers(board);
    switch (board->difficulty) {
        case EASY:
            makeRealMove(board, randomMovement(board, player));
            break;
        case INTERMEDIATE:
            makeRealMove(board, bestMove(board, player));
            break;
        case HARD:
            makeRealMove(board, bestMinimaxMove(board, player));
            break;
    }
}

/**
 * Initialize a board
 * @param board Receives a Board type structure
 */
void initializeBoard(Board *board) {

    board->lastPiecetypeMoved = WHITE_PLAYER;

    board->state = (Piece **) malloc(board->size * sizeof(Piece *));
    for (int i = 0; i < board->size; i++) {
        board->state[i] = (Piece *) malloc(board->size * sizeof(Piece));
    }
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            board->state[i][j].pieceType = VOID;
        }
    }

    board->initialState = (Piece **) malloc(board->size * sizeof(Piece *));
    for (int i = 0; i < board->size; i++) {
        board->initialState[i] = (Piece *) malloc(board->size * sizeof(Piece));
    }
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            board->initialState[i][j].pieceType = VOID;
        }
    }

    board->state[board->size / 2 - 1][board->size / 2 - 1].pieceType = WHITE_PIECE;
    board->state[board->size / 2][board->size / 2].pieceType = WHITE_PIECE;
    board->state[board->size / 2 - 1][board->size / 2].pieceType = BLACK_PIECE;
    board->state[board->size / 2][board->size / 2 - 1].pieceType = BLACK_PIECE;

    board->initialState[board->size / 2 - 1][board->size / 2 - 1].pieceType = WHITE_PIECE;
    board->initialState[board->size / 2][board->size / 2].pieceType = WHITE_PIECE;
    board->initialState[board->size / 2 - 1][board->size / 2].pieceType = BLACK_PIECE;
    board->initialState[board->size / 2][board->size / 2 - 1].pieceType = BLACK_PIECE;

}

/**
 * Initialize Custom Board
 * @param board receives a Board type structure
 */
void setCustomBoardState(Board *board) {
    for (int k = 0; k < board->size; k++) {
        for (int l = 0; l < board->size; l++) {
            board->state[k][l].pieceType = board->initialState[k][l].pieceType;
        }
    }
}


/**
 * Check if the game is finished
 * @param board Receives a Board type structure
 * @return 1 if the game is finished, 0 game still running
 */
bool isGameOver(Board *board) {
    return !canMove(board, BLACK_PLAYER) && !canMove(board, WHITE_PLAYER);
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
            if (board->state[i][j].pieceType == WHITE_PLAYER)
                white_moves++;
            if (board->state[i][j].pieceType == BLACK_PLAYER)
                black_moves++;
        }
    }
    if (white_moves == black_moves)
        return TIE;
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
bool canGoBack(Board *board) {
    return board->noOfMovesBack > 0;
}

/**
 * Determine if I can go foward a move
 * @param board Receives a Board type structure
 * @return The number of moves I can go foward
 */
bool canGoFoward(Board *board) {
    return board->noOfMovesFoward > 0;
}

/**
 * Determine the possible moves that the player can make
 * @param board Receives a Board type structure
 */
void SetHelpers(Board *board, PlayerType player) {
    int possibleMoves = 0;
    cleanHelpers(board);
    PlayerType opponent = player == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
    if (board->lastPiecetypeMoved == player && canMove(board, opponent))
        return;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j].pieceType == VOID) {
                Movement m = {.pieceType = player, .x = i, .y = j};
                if (isValidMove(board, m)) {
                    possibleMoves++;
                    board->state[i][j].pieceType = HELPER;
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
            if (board->state[i][j].pieceType == piece)
                score++;
        }
    }
    return score;
}

/**
 * Evaluates if there are not more possible moves for Black Piece, if the White Piece
 * @param board receives a Board type structure
 * @return 1 if the game is over, the last moved piece is White and there is no forward moves, otherwise, returns 0
 */
int canSkipBlackPiece(Board *board) {
    return !isGameOver(board) && !canMove(board, BLACK_PLAYER) && board->lastPiecetypeMoved == WHITE_PLAYER &&
           board->noOfMovesFoward == 0;
}

void cleanHelpers(Board *board){
    int bestScore = 0, x, y;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j].pieceType == HELPER) {
                board->state[i][j].pieceType = VOID;
            }
        }
    }
}

/**
 * Evaluate the best possible movement for the CPU
 * @param board Receives a Board type structure
 * @return A structure with the coordinates of the best movement
 */
Movement bestMove(Board *board, PlayerType player) {
    int bestScore = 0, x, y;

    Movement *allMoves = getAllPossibleMoves(board, player);
    for (int i = 0; i < getNumberOfMoves(board, player); i++) {
        Board tmp = copyBoard(*board);

        Movement m = {.pieceType = player, .x = allMoves[i].x, .y = allMoves[i].y};

        makeMove(&tmp, m);
        if (getScore(&tmp, player) - getScore(board, player) > bestScore) {
            bestScore = getScore(&tmp, player) - getScore(board, player);
            x = m.x;
            y = m.y;

        }
        destructBoard(&tmp);
    }
    allMoves = realloc(allMoves, 0);
    Movement m = {.pieceType = player, .x = x, .y = y};

    return m;
}

/**
 * Evaluates the best possible movement for the CPU using MiniMax algorithm
 * @param board Receives a Board type structure
 * @return A structure with the coordinates of the best movement using MiniMax
 */
Movement bestMinimaxMove(Board *board, PlayerType player) {
    nodes = 0;

    Movement *allMoves = getAllPossibleMoves(board, player);
    Movement bestMove = (Movement) {.pieceType = NONE, .x = -1, .y=1};
    int score = INT_MIN;
    int numberOfMoves = getNumberOfMoves(board, player);

    Board *boards = malloc(sizeof(Board) * numberOfMoves);
    for (int i = 0; i < numberOfMoves; i++) {
        Board tmp = copyBoard(*board);

        Movement m = {.pieceType = player, .x = allMoves[i].x, .y = allMoves[i].y};

        int scoreTemp = MinimaxSolver(5, (int) INT_MIN, INT_MAX, &tmp, m, player);
        if (scoreTemp > score) {
            score = scoreTemp;
            bestMove = m;
        }
        destructBoard(&tmp);
    }

    allMoves = realloc(allMoves, 0);
    printf("Nodes searched = %d\n", nodes);
    return bestMove;

}

/**
 * Evaluates a random movement for the CPU
 * @param board Receives a Board type structure
 * @return A structure with the coordinates of a random movement.
 */
Movement randomMovement(Board *board, PlayerType player) {
    int possibleMoves = getNumberOfMoves(board, player);
    Movement *moves = getAllPossibleMoves(board, player);
    Movement move;
    if (possibleMoves > 0)
        move = moves[rand() % possibleMoves];
    moves = realloc(moves, 0);
    return move;
}

/**
 * Evaluates all the Possible Movements for CPU and player.
 * @param board Receives a Board type structure
 * @param
 * @return A structure with the coordinates of a random movement.
 */
Movement *getAllPossibleMoves(Board *board, int pieceType) {
    int possibleMoves = 0;
    Movement *moves = malloc(sizeof(Movement));
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j].pieceType == VOID || board->state[i][j].pieceType == HELPER) {
                Movement m = {.pieceType = pieceType, .x = i, .y = j};
                if (isValidMove(board, m)) {
                    moves = realloc(moves, (possibleMoves + 1) * sizeof(Movement));
                    moves[possibleMoves] = m;
                    possibleMoves++;
                }
            }
        }
    }
    return moves;
}

/**
 *
 * @param board Receives a Board type structure
 * @param pieceType Receives an integer PieceType
 * @return The number of moves
 */
int getNumberOfMoves(Board *board, int pieceType) {
    int moves = 0;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j].pieceType == VOID || board->state[i][j].pieceType == HELPER) {
                Movement m_black = {.pieceType = pieceType, .x = i, .y = j};
                if (isValidMove(board, m_black))
                    moves++;
            }
        }
    }
    return moves;
}

/**
 * Evaluate if current movement is valid
 * @param board Receives a Board type structure
 * @param lastMove a Movement type structure
 * @return 1 if it is a valid move, 0 when all hopes fade away
 */
bool isValidMove(Board *board, Movement lastMove) {
    char opponent = (lastMove.pieceType == WHITE_PLAYER) ? BLACK_PLAYER : WHITE_PLAYER;

    int colIndex, rowIndex, offset;

    colIndex = lastMove.x - 1;
    rowIndex = lastMove.y;
    offset = 0;
    while (colIndex > 0 && board->state[colIndex][rowIndex].pieceType == opponent) {
        colIndex--;
        offset++;
    }
    if (colIndex >= 0 && board->state[colIndex][rowIndex].pieceType == lastMove.pieceType && offset > 0)
        return true;

    //move down
    colIndex = lastMove.x + 1;
    rowIndex = lastMove.y;
    offset = 0;
    while (colIndex < board->size - 1 && board->state[colIndex][rowIndex].pieceType == opponent) {
        colIndex++;
        offset++;
    }
    if (colIndex <= board->size - 1 && board->state[colIndex][rowIndex].pieceType == lastMove.pieceType &&
        offset > 0)
        return true;

    //move left
    colIndex = lastMove.x;
    rowIndex = lastMove.y - 1;
    offset = 0;
    while (rowIndex > 0 && board->state[colIndex][rowIndex].pieceType == opponent) {
        rowIndex--;
        offset++;
    }
    if (rowIndex >= 0 && board->state[colIndex][rowIndex].pieceType == lastMove.pieceType && offset > 0)
        return true;

    //move right
    colIndex = lastMove.x;
    rowIndex = lastMove.y + 1;
    offset = 0;
    while (rowIndex < board->size - 1 && board->state[colIndex][rowIndex].pieceType == opponent) {
        rowIndex++;
        offset++;
    }
    if (rowIndex <= board->size - 1 && board->state[colIndex][rowIndex].pieceType == lastMove.pieceType &&
        offset > 0)
        return true;

    //move up left
    colIndex = lastMove.x - 1;
    rowIndex = lastMove.y - 1;
    offset = 0;
    while (colIndex > 0 && rowIndex > 0 && board->state[colIndex][rowIndex].pieceType == opponent) {
        colIndex--;
        rowIndex--;
        offset++;
    }
    if (colIndex >= 0 && rowIndex >= 0 && board->state[colIndex][rowIndex].pieceType == lastMove.pieceType &&
        offset > 0)
        return true;

    //move up right
    colIndex = lastMove.x - 1;
    rowIndex = lastMove.y + 1;
    offset = 0;
    while (colIndex > 0 && rowIndex < board->size - 1 && board->state[colIndex][rowIndex].pieceType == opponent) {
        colIndex--;
        rowIndex++;
        offset++;
    }
    if (colIndex >= 0 && rowIndex <= board->size - 1 &&
        board->state[colIndex][rowIndex].pieceType == lastMove.pieceType && offset > 0)
        return true;

    //move down left
    colIndex = lastMove.x + 1;
    rowIndex = lastMove.y - 1;
    offset = 0;
    while (colIndex < board->size - 1 && rowIndex > 0 && board->state[colIndex][rowIndex].pieceType == opponent) {
        colIndex++;
        rowIndex--;
        offset++;
    }
    if (colIndex <= board->size - 1 && rowIndex >= 0 &&
        board->state[colIndex][rowIndex].pieceType == lastMove.pieceType && offset > 0)
        return true;

    //move down right
    colIndex = lastMove.x + 1;
    rowIndex = lastMove.y + 1;
    offset = 0;
    while (colIndex < board->size - 1 && rowIndex < board->size - 1 &&
           board->state[colIndex][rowIndex].pieceType == opponent) {
        colIndex++;
        rowIndex++;
        offset++;
    }
    if (colIndex <= board->size - 1 && rowIndex <= board->size - 1 &&
        board->state[colIndex][rowIndex].pieceType == lastMove.pieceType && offset > 0)
        return true;

    //when all hopes fade away
    return false;
}

/**
 * Determine if current piece can make a move
 * @param board Receives a Board type structure
 * @param pieceType Receives a pieceType type int
 * @return 1 if I can make a move, 0 when all hopes fade away
 */
bool canMove(Board *board, int pieceType) {
    return getNumberOfMoves(board, pieceType);
}

/**
 * Makes a copy of the current Board.
 * @param board Receives a Board type structure
 * @return the copy of the Board type structure
 */
Board copyBoard(Board board) {
    Board tmp;
    initializeGame(&tmp, board.size, board.difficulty, board.custom, board.player1, board.player2);

    if (board.custom) {
        for (int k = 0; k < board.size; k++) {
            for (int l = 0; l < board.size; l++) {
                tmp.initialState[k][l].pieceType = board.initialState[k][l].pieceType;
            }
        }
        setCustomBoardState(&tmp);
    }
    for (int k = 0; k < board.size; k++) {
        for (int l = 0; l < board.size; l++) {
            tmp.state[k][l].pieceType = board.state[k][l].pieceType;
        }
    }
    return tmp;
}

PlayerType nextTurn(Board *board) {
    if (!isGameOver(board)) {
        PlayerType next = board->lastPiecetypeMoved == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
        if (board->noOfMovesFoward <= 0) {
            if (canMove(board, next)) {
                return next;
            } else return board->lastPiecetypeMoved;
        } else {
            if (board->player1.isHuman == true && next==BLACK_PLAYER) return BLACK_PLAYER;
            if (board->player2.isHuman == true && next==WHITE_PLAYER) return WHITE_PLAYER;
            return NONE;
        }
    } else return NONE;
}

/**
 *
 */
int MinimaxSolver(int depth, int alpha, int beta, Board *board1, Movement moveEval, PlayerType player) {
    nodes++;

    makeMove(board1, moveEval);
    if (depth == 0) {
        return getPointEvaluator(board1, moveEval.pieceType);
    } else if (isGameOver(board1)) {
        return getScore(board1, moveEval.pieceType);
    }

    int max = moveEval.pieceType == player;

    char opponent = (moveEval.pieceType == WHITE_PLAYER) ? BLACK_PLAYER : WHITE_PLAYER;

    if ((max && !canMove(board1, WHITE_PLAYER)) || (!max && !canMove(board1, opponent))) {
        return MinimaxSolver(
                depth - 1, alpha, beta, board1, moveEval, player);
    }

    Board board = copyBoard(*board1);
    int totalScore;
    if (max) {
        totalScore = INT_MIN;
        Movement *all = getAllPossibleMoves(&board, WHITE_PLAYER);
        int numberOfMoves = getNumberOfMoves(&board, WHITE_PLAYER);
        for (int i = 0; i < numberOfMoves; ++i) {
            Movement move = all[i];
            int score = MinimaxSolver(depth - 1, alpha, beta, &board, move, player);
            if (score > totalScore) {
                totalScore = score;
            }
            alpha = (int) fmax(alpha, score);
            if (beta <= alpha) {
                break;
            }
        }
        all = realloc(all, 0);
    } else {
        totalScore = INT_MAX;
        Movement *all = getAllPossibleMoves(&board, BLACK_PLAYER);
        int numberOfMoves = getNumberOfMoves(&board, BLACK_PLAYER);
        for (int i = 0; i < numberOfMoves; ++i) {
            Movement move = all[i];
            int score = MinimaxSolver(depth - 1, alpha, beta, &board, move, player);
            if (score < totalScore) {
                totalScore = score;
            }
            beta = (int) fmin(alpha, score);
            if (beta <= alpha) {
                break;
            }
        }
        all = realloc(all, 0);

    }
    destructBoard(&board);
    return totalScore;
}

/**
 * Destroys the current board
 * @param board Receives a Board type structure
 */
void destructBoard(Board *board) {
    for (int i = 0; i < board->size; i++) {

        board->state[i] = realloc(board->state[i], 0);

    }

    for (int i = 0; i < board->size; i++) {

        board->initialState[i] = realloc(board->initialState[i], 0);

    }
    board->state = realloc(board->state, 0);
    board->state = realloc(board->initialState, 0);
    board->state = NULL;
    board->historyBack = realloc(board->historyBack, 0);
    board->historyForward = realloc(board->historyForward, 0);
    board = NULL;
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
        Board boardTmp;
        if (board->custom) {
            boardTmp = copyBoard(*board);
        }
        board->historyForward = realloc(board->historyForward, sizeof(Movement) * (board->noOfMovesFoward + 1));
        board->historyForward[board->noOfMovesFoward] = board->historyBack[board->noOfMovesBack - 1];
        board->noOfMovesFoward += 1;
        int moves = board->noOfMovesBack;
        initializeBoard(board);
        if (boardTmp.custom) {
            for (int i = 0; i < board->size; i++) {
                for (int j = 0; j < board->size; j++) {
                    board->initialState[i][j].pieceType = boardTmp.initialState[i][j].pieceType;
                }
            }
            setCustomBoardState(board);
            destructBoard(&boardTmp);
        }
        board->historyBack = realloc(board->historyBack, sizeof(Movement) * (moves - 1));
        board->noOfMovesBack = 0;
        for (int i = 0; i < moves - 1; ++i) {
            makeRealMove(board, m[i]);
        }
        m = realloc(m, 0);
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
        Board boardTmp;
        if (board->custom) {
            boardTmp = copyBoard(*board);
        }
        initializeBoard(board);
        if (boardTmp.custom) {
            for (int i = 0; i < board->size; i++) {
                for (int j = 0; j < board->size; j++) {
                    board->initialState[i][j].pieceType = boardTmp.initialState[i][j].pieceType;
                }
            }
            setCustomBoardState(board);
            destructBoard(&boardTmp);
        }
        board->historyForward = realloc(board->historyForward, sizeof(Movement) * (movesForward));
        board->historyForward = m;
        board->noOfMovesBack = 0;
        for (int i = 0; i < moves; ++i) {
            makeRealMove(board, historyRebuild[i]);
        }
        historyRebuild = realloc(historyRebuild, 0);
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
    if (lastMove.x < 0 || lastMove.x > board->size - 1 || lastMove.y < 0 || lastMove.y > board->size - 1 ||
        !(lastMove.pieceType == BLACK_PLAYER || lastMove.pieceType == WHITE_PLAYER))
        return;
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
    char opponent = (lastMove.pieceType == WHITE_PLAYER) ? BLACK_PLAYER : WHITE_PLAYER;

    int rowIndex, colIndex;
    Board tmp = copyBoard(*board);

    tmp.state[lastMove.x][lastMove.y].pieceType = lastMove.pieceType;

    //move down
    Movement *moves;
    moves = malloc(sizeof(Movement));
    rowIndex = lastMove.x - 1;
    colIndex = lastMove.y;
    int counter = 0;
    while (rowIndex > 0 && board->state[rowIndex][colIndex].pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex--;
    }
    if (rowIndex >= 0 && board->state[rowIndex][colIndex].pieceType == lastMove.pieceType && counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y].pieceType = lastMove.pieceType;
        }
    }

    //move up
    rowIndex = lastMove.x + 1;
    colIndex = lastMove.y;
    counter = 0;
    while (rowIndex < board->size - 1 && board->state[rowIndex][colIndex].pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex++;
    }
    if (rowIndex <= board->size - 1 && board->state[rowIndex][colIndex].pieceType == lastMove.pieceType &&
        counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y].pieceType = lastMove.pieceType;
        }
    }

    //move right
    rowIndex = lastMove.x;
    colIndex = lastMove.y - 1;
    counter = 0;
    while (colIndex > 0 && board->state[rowIndex][colIndex].pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        colIndex--;
    }
    if (colIndex >= 0 && board->state[rowIndex][colIndex].pieceType == lastMove.pieceType && counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y].pieceType = lastMove.pieceType;
        }
    }

    //move left
    rowIndex = lastMove.x;
    colIndex = lastMove.y + 1;
    counter = 0;
    while (colIndex < board->size - 1 && board->state[rowIndex][colIndex].pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        colIndex++;
    }
    if (colIndex <= board->size - 1 && board->state[rowIndex][colIndex].pieceType == lastMove.pieceType &&
        counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y].pieceType = lastMove.pieceType;
        }
    }

    //move down right
    rowIndex = lastMove.x - 1;
    colIndex = lastMove.y - 1;
    counter = 0;
    while (rowIndex > 0 && colIndex > 0 && board->state[rowIndex][colIndex].pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex--;
        colIndex--;
    }
    if (rowIndex >= 0 && colIndex >= 0 && board->state[rowIndex][colIndex].pieceType == lastMove.pieceType &&
        counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y].pieceType = lastMove.pieceType;
        }
    }

    //move up right
    rowIndex = lastMove.x - 1;
    colIndex = lastMove.y + 1;
    counter = 0;
    while (rowIndex > 0 && colIndex < board->size - 1 && board->state[rowIndex][colIndex].pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex--;
        colIndex++;
    }
    if (rowIndex >= 0 && colIndex <= board->size - 1 &&
        board->state[rowIndex][colIndex].pieceType == lastMove.pieceType && counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y].pieceType = lastMove.pieceType;
        }
    }

    //move down left
    rowIndex = lastMove.x + 1;
    colIndex = lastMove.y - 1;
    counter = 0;
    while (rowIndex < board->size - 1 && colIndex > 0 && board->state[rowIndex][colIndex].pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex++;
        colIndex--;
    }
    if (rowIndex <= board->size - 1 && colIndex >= 0 &&
        board->state[rowIndex][colIndex].pieceType == lastMove.pieceType && counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y].pieceType = lastMove.pieceType;
        }
    }

    //move up left
    rowIndex = lastMove.x + 1;
    colIndex = lastMove.y + 1;
    counter = 0;
    while (rowIndex < board->size - 1 && colIndex < board->size - 1 &&
           board->state[rowIndex][colIndex].pieceType == opponent) {
        moves = realloc(moves, (counter + 1) * sizeof(Movement));
        Movement m = (Movement) {lastMove.pieceType, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex++;
        colIndex++;
    }
    if (rowIndex <= board->size - 1 && colIndex <= board->size - 1 &&
        board->state[rowIndex][colIndex].pieceType == lastMove.pieceType && counter > 0) {
        for (int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y].pieceType = lastMove.pieceType;
        }
    }

    for (int k = 0; k < board->size; k++) {
        for (int l = 0; l < board->size; l++) {
            board->state[k][l].pieceType = tmp.state[k][l].pieceType;
        }
    }
    moves = realloc(moves, 0);
    destructBoard(&tmp);
}

/**
 * Save current game
 * @param board Receives a Board type structure
 */

char *saveGame(Board *board) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "board_size", board->size);
    cJSON_AddNumberToObject(json, "game_difficulty", board->difficulty);
    cJSON *movements = cJSON_AddArrayToObject(json, "movements");
    for (int i = 0; i < board->noOfMovesBack; i++) {
        cJSON *object = cJSON_CreateObject();
        cJSON_AddNumberToObject(object, "piece_type", board->historyBack[i].pieceType);
        cJSON_AddNumberToObject(object, "x", board->historyBack[i].x);
        cJSON_AddNumberToObject(object, "y", board->historyBack[i].y);
        cJSON_AddItemToArray(movements, object);
    }
    cJSON_AddNumberToObject(json, "custom", board->custom);
    if (board->custom) {
        cJSON *inital_board = cJSON_AddArrayToObject(json, "initial_board");

        for (int i = 0; i < board->size; i++) {
            for (int j = 0; j < board->size; ++j) {
                cJSON *object = cJSON_CreateObject();
                cJSON_AddNumberToObject(object, "piece_type", board->initialState[i][j].pieceType);
                cJSON_AddItemToArray(inital_board, object);
            }
        }

    }
    return cJSON_PrintUnformatted(json);
}

/**
 * Load a saved game
 * @param data receives a char pointer to data
 */
Board loadGame(char *data) {
    Board board;
    cJSON *json = cJSON_Parse(data);
    if (json != NULL) {
        cJSON *board_size = cJSON_GetObjectItemCaseSensitive(json, "board_size");
        cJSON *game_difficulty = cJSON_GetObjectItemCaseSensitive(json, "game_difficulty");
        cJSON *is_custom = cJSON_GetObjectItemCaseSensitive(json, "custom");
        if (cJSON_IsNumber(board_size) && board_size != NULL, cJSON_IsNumber(game_difficulty) &&
                                                              game_difficulty != NULL) {
            initializeGame(&board, board_size->valueint, game_difficulty->valueint, is_custom->valueint,(Player) {true}, (Player) {false});
            if (is_custom->valueint) {
                cJSON *piece = NULL;
                int count = 0;
                cJSON *inital_board = cJSON_GetObjectItemCaseSensitive(json, "initial_board");
                cJSON_ArrayForEach(piece, inital_board) {
                    cJSON *pieceType = cJSON_GetObjectItemCaseSensitive(piece, "piece_type");
                    board.initialState[count / board_size->valueint][count % board_size->valueint] = (Piece) {
                            pieceType->valueint};
                    count++;
                }
                setCustomBoardState(&board);
            }

            cJSON *move_obj = NULL;
            cJSON *move_array = cJSON_GetObjectItemCaseSensitive(json, "movements");
            cJSON_ArrayForEach(move_obj, move_array) {
                cJSON *pieceType = cJSON_GetObjectItemCaseSensitive(move_obj, "piece_type");
                cJSON *x = cJSON_GetObjectItemCaseSensitive(move_obj, "x");
                cJSON *y = cJSON_GetObjectItemCaseSensitive(move_obj, "y");
                Movement m = {pieceType->valueint, x->valueint, y->valueint};
                makeRealMove(&board, m);
            }
        }
    }
    return board;
}
