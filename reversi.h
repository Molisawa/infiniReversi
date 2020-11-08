/*Function prototypes*/
typedef  struct piece_struct{
    int player;
    int x, y;
    char pieceType
}Piece;

typedef struct board_struct{
    Piece* state[8][8];
    int currentPlayer;
    int noOfGames;
    int noOfMoves;
    int invalidMoves;
    int compScore;
    int userScore;
}Board;

/*
 *      board[SIZE/2 - 1][SIZE/2 - 1] = board[SIZE/2][SIZE/2] = 'O';
        board[SIZE/2 - 1][SIZE/2] = board[SIZE/2][SIZE/2 - 1] = 'X';
        Calculos para sacar la posicion de las fichas en cualquier tablero
 *
 * */
void playReversi();
void initializeGame(Board* b,int SIZE);
void initializeBoard(Board* Board, int SIZE);
void endGame();
void saveGame();
void loadGame();
void display();
int isValidMove(Board currentState, int player);
void make_move();
void computer_move(Board* board,int SIZE);
int best_move();
int get_score();