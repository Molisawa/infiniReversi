/*Function prototypes*/
typedef  struct piece_struct{
    int player;
    int x, y;
}Piece;

typedef struct board_struct{
    Piece* state[][];
    int currentPlayer;
}Board;

/*
 *      board[SIZE/2 - 1][SIZE/2 - 1] = board[SIZE/2][SIZE/2] = 'O';
        board[SIZE/2 - 1][SIZE/2] = board[SIZE/2][SIZE/2 - 1] = 'X';
        Calculos para sacar la posicion de las fichas en cualquier tablero
 *
 * */
void playReversi();
void initializeGame();
void endGame();
void saveGame();
void loadGame();
void display();
int valid_moves();
void make_move();
void computer_move();
int best_move();
int get_score();