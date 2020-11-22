// Must be even number
#ifndef REVERSI_GAME_ENGINE_LIBRARY_H
#define REVERSI_GAME_ENGINE_LIBRARY_H

#include <stdbool.h>

enum StateFlags {
    VOID,
    BLACK_PIECE,
    WHITE_PIECE,
    HELPER
};

enum winners {
    WINNER,
    LOSER,
    TIE
};

enum difficulty {
    EASY,
    INTERMEDIATE,
    HARD
};

typedef struct piece_struct {
    enum StateFlags pieceType;
} Piece;

typedef struct movement {
    enum StateFlags pieceType;
    int x, y;
} Movement;

typedef struct board_struct {
    Piece **state;
    enum difficulty difficulty;
    Movement *historyBack;
    Movement *historyForward;
    int noOfMovesBack;
    int lastPiecetypeMoved;
    int noOfMovesFoward;
    bool initialized;
    int size;
    bool custom;
    Piece **initialState;
} Board;


void setDefaultInitialBoardState(Board *board);

void setCustomBoardState(Board *board);

void initializeGame(Board *b, int size, int difficulty, bool custom);

void initializeBoard(Board *board);

int getNumberOfMoves(Board *board, int pieceType);

int getPointEvaluator(Board *board, int pieceType);

int getScorePosition(Board *board, int pieceType);

Movement *getAllPossibleMoves(Board *board, int pieceType);

void goBack(Board *board);

void goForward(Board *board);

bool canGoBack(Board *board);

bool canGoFoward(Board *board);

void endGame();

char *saveGame(Board *board);

Board loadGame(char *filename);

bool isValidMove(Board *board, Movement lastMove);

bool isGameOver(Board *board);

int getWinner(Board *board);

void SetHelpers(Board *board);

void makeMove(Board *board, Movement lastMove);

void makeRealMove(Board *board, Movement lastMove);

void removeHistoryFoward(Board *board);

Movement randomMovement(Board *board);

Board copyBoard(Board board);

void destructBoard(Board *board);

int MinimaxSolver(int depth, int alpha, int beta, Board *board, Movement move);

Movement bestMinimaxMove(Board *board);

void computerMove(Board *board);

bool canMove(Board *board, int pieceType);

Movement bestMove(Board *board);

int canSkipBlackPiece(Board *board);

int getScore(Board *board, int pieceType);

#endif