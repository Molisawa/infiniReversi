#ifndef REVERSI_GAME_ENGINE_LIBRARY_H
#define REVERSI_GAME_ENGINE_LIBRARY_H

#include <stdbool.h>

typedef enum {
    BLACK_PLAYER = 1,
    WHITE_PLAYER = 2,
    NONE = 6
} PlayerType;

typedef enum {
    VOID,
    BLACK_PIECE = BLACK_PLAYER,
    WHITE_PIECE = WHITE_PLAYER,
    HELPER,
} StateFlags;

typedef enum {
    WINNER,
    LOSER,
    TIE
} Winners;

typedef enum {
    EASY,
    INTERMEDIATE,
    HARD
} Difficulty;

typedef struct {
    StateFlags pieceType;
} Piece;

typedef struct {
    PlayerType pieceType;
    int x, y;
} Movement;

typedef struct {
    bool isHuman;
} Player;

typedef struct {
    Piece **state;
    Difficulty difficulty;
    Movement *historyBack;
    Movement *historyForward;
    int noOfMovesBack;
    PlayerType lastPiecetypeMoved;
    int noOfMovesFoward;
    bool initialized;
    int size;
    bool custom;
    Piece **initialState;
    Player player1;
    Player player2;
} Board;


void setDefaultInitialBoardState(Board *board);

void setCustomBoardState(Board *board);

void initializeGame(Board *board, int size, int difficulty, bool custom, Player player1, Player player2);

void initializeBoard(Board *board);

int getNumberOfMoves(Board *board, int pieceType);

int getPointEvaluator(Board *board, int pieceType);

int getScorePosition(Board *board, int pieceType);

Movement *getAllPossibleMoves(Board *board, int pieceType);

void goBack(Board *board);

void goForward(Board *board);

bool canGoBack(Board *board);

bool canGoFoward(Board *board);

PlayerType nextTurn(Board *board);

void endGame();

char *saveGame(Board *board);

Board loadGame(char *filename);

bool isValidMove(Board *board, Movement lastMove);

bool isGameOver(Board *board);

int getWinner(Board *board);

void SetHelpers(Board *board, PlayerType player);

void cleanHelpers(Board *board);

void makeMove(Board *board, Movement lastMove);

void makeRealMove(Board *board, Movement lastMove);

void removeHistoryFoward(Board *board);

Movement randomMovement(Board *board, PlayerType);

Board copyBoard(Board board);

void destructBoard(Board *board);

int MinimaxSolver(int depth, int alpha, int beta, Board *board, Movement move, PlayerType);

Movement bestMinimaxMove(Board *board, PlayerType);

void computerMove(Board *board, PlayerType player);

bool canMove(Board *board, int pieceType);

Movement bestMove(Board *board, PlayerType player);

int canSkipBlackPiece(Board *board);

int getScore(Board *board, int pieceType);

#endif