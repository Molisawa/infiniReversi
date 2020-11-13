/*Function prototypes*/
#define BOARD_SIZE 8

enum StateFlags {VOID,BLACK_PIECE,WHITE_PIECE,HELPER};// Must be even number
typedef  struct piece_struct{
    enum StateFlags pieceType;
}Piece;


typedef struct movement{
    const enum StateFlags pieceState;
    const int x, y;
} Movement;

typedef struct board_struct{
    Piece* state[BOARD_SIZE][BOARD_SIZE];
    Movement* history;
    char currentPlayer;
    int noOfGames;
    int noOfMoves;
    int invalidMoves;
    int compScore;
    int userScore;
    int Size;
}Board;

int playReversi();
Movement askMove();
void initializeGame(Board* b);
void initializeBoard(Board* Board);
void endGame();
void saveGame();
void loadGame();
void display();
int isValidMove(Board currentState, int player);
void make_move(Board* board, Movement lastMove);
void computer_move();
int best_move();
int get_score();
