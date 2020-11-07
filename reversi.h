/*Function prototypes*/
typedef  struct piece_struct{
    int player;
    int x, y;
}Piece;

typedef struct board_struct{
    Piece* state[][];
    int currentPlayer;
}Board;
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