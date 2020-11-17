// Must be even number
#ifndef REVERSI_GAME_ENGINE_LIBRARY_H
#define REVERSI_GAME_ENGINE_LIBRARY_H
enum StateFlags
{
    VOID,
    BLACK_PIECE,
    WHITE_PIECE,
    HELPER
};

enum winners
{
    WINNER,
    LOSER,
    TIE
};

enum difficulty
{
    EASY,
    INTERMEDIATE,
    HARD
};

typedef struct piece_struct
{
    enum StateFlags pieceType;
} Piece;

typedef struct movement
{
    enum StateFlags pieceType;
    int x, y;
} Movement;

typedef struct board_struct
{
    Piece **state;
    enum difficulty difficulty;
    Movement *historyBack;
    Movement *historyForward;
    int noOfMovesBack;
    int lastPiecetypeMoved;
    int noOfMovesFoward;
    int initialized;
    int size;
} Board;

typedef struct minimax
{
    Movement m;
    Board *board;
    int score;
}Minimax;


void initializeGame(Board *b, int size, int difficulty);

void initializeBoard(Board *board);

int getNumberOfMoves(Board *board, int pieceType);

Movement *getAllPossibleMoves(Board *board, int pieceType);

void goBack(Board *board);

void goForward(Board *board);

int canGoBack(Board *board);

int canGoFoward(Board *board);

void endGame();

char *saveGame(Board *board);

Board loadGame(char *filename);

int isValidMove(Board *board, Movement lastMove);

int isGameOver(Board *board);

int getWinner(Board *board);

void SetHelpers(Board *board);

void makeMove(Board *board, Movement lastMove);

void makeRealMove(Board *board, Movement lastMove);

void removeHistoryFoward(Board *board);

Movement randomMovement(Board *board);

Board buildGameState(Board board, Movement* moves,int movesCount);

Board copyBoard(Board board);

void destructBoard(Board* board);

Minimax MinimaxSolver(Minimax minimax, int depth, int alpha, int beta);

Movement bestMinimaxMove(Board *board);

void computerMove(Board *board);

int canMove(Board *board, int Piece);

Movement bestMove(Board *board);

int canSkipBlackPiece(Board *board);

int getScore(Board *board, int pieceType);

#endif