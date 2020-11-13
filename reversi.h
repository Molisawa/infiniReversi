#define BOARD_SIZE 8 // Must be even number

enum StateFlags {VOID,BLACK_PIECE,WHITE_PIECE,HELPER};
typedef  struct piece_struct{
    enum StateFlags pieceType;
}Piece;


typedef struct movement{
    enum StateFlags pieceState;
    int x, y;
} Movement;

typedef struct board_struct{
    Piece* state[BOARD_SIZE][BOARD_SIZE];
    Movement* history;
    int *noOfMoves;
    int compScore;
    int userScore;
    int size;
}Board;

void initializeGame(Board *b);
void initializeBoard(Board *board);
void goBack(Board *board);
void endGame();
void saveGame();
void loadGame();
int isValidMove(Board *board, Movement lastMove);
int isGameOver(Board *board);
void SetHelpers(Board *board);
void make_move(Board *board, Movement lastMove);
void computer_move();
int best_move();
int get_score();