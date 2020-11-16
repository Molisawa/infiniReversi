#define BOARD_SIZE 8 // Must be even number
#ifndef REVERSI_LIBRARY_H
#define REVERSI_LIBRARY_H
enum StateFlags {
    VOID, BLACK_PIECE, WHITE_PIECE, HELPER
};

enum winners {
    WINNER, LOSER, TIE
};

typedef struct piece_struct {
    enum StateFlags pieceType;
} Piece;


typedef struct movement {
    enum StateFlags pieceType;
    int x, y;
} Movement;

typedef struct board_struct {
    Piece *state[BOARD_SIZE][BOARD_SIZE];
    Movement *historyBack;
    Movement *historyForward;
    int noOfMovesBack;
    int lastPiecetypeMoved;
    int noOfMovesFoward;
    int compScore;
    int userScore;
    int size;
} Board;

void initializeGame(Board *b);

void initializeBoard(Board *board);

void goBack(Board *board);

void goForward(Board *board);

int canGoBack(Board *board);

int canGoFoward(Board *board);

void endGame();

void saveGame(Board *board);

void loadGame();

int isValidMove(Board *board, Movement lastMove);

int isGameOver(Board *board);

int getWinner(Board *board);

void SetHelpers(Board *board);

void makeMove(Board *board, Movement lastMove);

void makeRealMove(Board *board, Movement lastMove);

void removeHistoryFoward(Board *board);

void computerMove(Board *board);

int canMove(Board *board, int Piece);

Movement bestMove(Board *board);

int canSkipBlackPiece(Board *board);

int getScore(Board *board, int pieceType);

#endif