#include <stdlib.h>
#include "reversi.h"


void initializeGame(Board *board){
    board->compScore = 0;
    board->userScore = 0;
    board->noOfMovesBack = 0;
    board->historyBack = malloc(sizeof(Movement));
    board->noOfMovesFoward = 0;
    board->historyForward = malloc(sizeof(Movement));
    board->size = BOARD_SIZE;
    initializeBoard(board);
}

void computerMove(Board *board){
    puts("entro compu");
    makeMove(board, bestMove(board));
    puts("salio compu");
}

void initializeBoard(Board *board){

    int SIZE = board->size;

    for(int i=0;i< board->size; i++)
    {
        for(int j=0;j< board->size; j++)
        {
            board->state[i][j] = malloc(sizeof(Piece));
            board->state[i][j]->pieceType = VOID;
        }
    }
    board->state[SIZE/2 - 1][SIZE/2 - 1]->pieceType = WHITE_PIECE;
    board->state[SIZE/2][SIZE/2]->pieceType = WHITE_PIECE;
    board->state[SIZE/2 - 1][SIZE/2]->pieceType = BLACK_PIECE;
    board->state[SIZE/2][SIZE/2 - 1]->pieceType = BLACK_PIECE;

}

int isGameOver(Board *board) {
    int white_moves = 0;
    int black_moves = 0;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j]->pieceType == VOID || board->state[i][j]->pieceType ==HELPER) {
                Movement m_black = {.pieceState = BLACK_PIECE, .x= i, .y=j};
                Movement m_white = {.pieceState = WHITE_PIECE, .x= i, .y=j};
                if (isValidMove(board, m_black))
                    black_moves++;
                if (isValidMove(board, m_white))
                    white_moves++;
            }
        }
    }
    if (white_moves == 0 && black_moves == 0) return 1;

    return 0;
}

int getWinner(Board *board){
    int white_moves = 0, black_moves = 0;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++){
            if (board->state[i][j]->pieceType ==WHITE_PIECE) white_moves++;
            if (board->state[i][j]->pieceType ==BLACK_PIECE) black_moves++;
        }
    }
    if (white_moves == black_moves) return TIE;
    if (white_moves>black_moves) {
        return LOSER;
    } else {
        return WINNER;
    }
}

int canGoBack(Board *board) {
    return board->noOfMovesBack > 0;
}

int canGoFoward(Board *board) {
    return board->noOfMovesFoward > 0;
}

void SetHelpers(Board *board){
    for(int i=0;i<board->size; i++){
        for(int j = 0; j < board->size; j++) {
            if(board->state[i][j]->pieceType == VOID) {
                Movement m = {.pieceState = BLACK_PIECE, .x= i, .y=j};
                if (isValidMove(board,m)) {
                    board->state[i][j]->pieceType = HELPER;
                }
            }
        }
    }
}

int getScore(Board *board, int piece){
    int score = 0;
    for(int i=0;i<board->size; i++){
        for(int j= 0; j < board->size; j++){
            if (board->state[i][j]->pieceType ==piece)
                score++;
        }
    }
    return score;
}

Movement bestMove(Board * board){
    int bestScore = 0,x,y;
    for(int i = 0; i < board->size; i++){
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j]->pieceType == HELPER) {
                board->state[i][j]->pieceType = VOID;
            }
        }
    }
    for(int i=0;i<board->size; i++){
        for(int j = 0; j < board->size; j++) {
            Board tmp;
            initializeGame(&tmp);
            for (int k = 0; k < board->size; k++) {
                for (int l = 0; l < board->size; l++){
                    tmp.state[k][l]->pieceType=board->state[k][l]->pieceType;
                }
            }
            if(tmp.state[i][j]->pieceType == VOID) {
                Movement m = {.pieceState = WHITE_PIECE, .x= i, .y=j};
                if (isValidMove(&tmp,m)){
                    makeMove(&tmp,m);
                    if (getScore(&tmp,WHITE_PIECE) - getScore(board,WHITE_PIECE)> bestScore){
                        bestScore = getScore(&tmp,WHITE_PIECE) - getScore(board,WHITE_PIECE);
                        x = i;
                        y = j;
                    }
                }
            }
        }
    }
    Movement m = {.pieceState = WHITE_PIECE, .x= x,.y=y};

    return m;
}

int isValidMove(Board *board, Movement lastMove){
    char opponent = (lastMove.pieceState == WHITE_PIECE) ? BLACK_PIECE : WHITE_PIECE;

    int colIndex , rowIndex , offset;

    colIndex = lastMove.x - 1;
    rowIndex = lastMove.y;
    offset = 0;
    while(colIndex > 0 && board->state[colIndex][rowIndex]->pieceType == opponent){
        colIndex--;
        offset++;
    }
    if(colIndex >= 0 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceState && offset > 0) return 1;


    //move down
    colIndex = lastMove.x + 1;
    rowIndex = lastMove.y;
    offset = 0;
    while(colIndex < board->size-1 && board->state[colIndex][rowIndex]->pieceType == opponent){
        colIndex++;
        offset++;
    }
    if(colIndex <= board->size-1 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceState && offset > 0) return 1;

    //move left
    colIndex = lastMove.x;
    rowIndex = lastMove.y - 1;
    offset = 0;
    while(rowIndex > 0 && board->state[colIndex][rowIndex]->pieceType == opponent){
        rowIndex--;
        offset++;
    }
    if(rowIndex >= 0 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceState && offset > 0) return 1;

    //move right
    colIndex = lastMove.x;
    rowIndex = lastMove.y + 1;
    offset = 0;
    while(rowIndex < board->size-1 && board->state[colIndex][rowIndex]->pieceType == opponent){
        rowIndex++;
        offset++;
    }
    if(rowIndex <= board->size-1 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceState && offset > 0) return 1;

    //move up left
    colIndex = lastMove.x - 1;
    rowIndex = lastMove.y - 1;
    offset = 0;
    while(colIndex > 0 && rowIndex > 0 && board->state[colIndex][rowIndex]->pieceType == opponent){
        colIndex--;
        rowIndex--;
        offset++;
    }
    if(colIndex >= 0 && rowIndex >= 0 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceState && offset > 0) return 1;

    //move up right
    colIndex = lastMove.x - 1;
    rowIndex = lastMove.y + 1;
    offset = 0;
    while(colIndex > 0 && rowIndex < board->size-1 && board->state[colIndex][rowIndex]->pieceType == opponent){
        colIndex--;
        rowIndex++;
        offset++;
    }
    if(colIndex >= 0 && rowIndex <= board->size-1 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceState && offset > 0) return 1;

    //move down left
    colIndex = lastMove.x + 1;
    rowIndex = lastMove.y - 1;
    offset = 0;
    while(colIndex < board->size-1 && rowIndex > 0 && board->state[colIndex][rowIndex]->pieceType == opponent){
        colIndex++;
        rowIndex--;
        offset++;
    }
    if(colIndex <= board->size-1 && rowIndex >= 0 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceState && offset > 0) return 1;

    //move down right
    colIndex = lastMove.x + 1;
    rowIndex = lastMove.y + 1;
    offset = 0;
    while(colIndex < board->size-1 && rowIndex < board->size-1 && board->state[colIndex][rowIndex]->pieceType == opponent){
        colIndex++;
        rowIndex++;
        offset++;
    }
    if(colIndex <= board->size-1 && rowIndex <= board->size-1 && board->state[colIndex][rowIndex]->pieceType == lastMove.pieceState && offset > 0) return 1;

    //when all hopes fade away
    return 0;
}

void goBack(Board *board) {
    if (canGoBack(board)) {
        Movement *m = malloc(sizeof(Movement) * (board->noOfMovesBack - 1));
        for (int i = 0; i < board->noOfMovesBack - 1; ++i) {
            m[i] = board->historyBack[i];
        }
        board->historyForward = realloc(board->historyForward, sizeof(Movement) * (board->noOfMovesFoward + 1));
        board->historyForward[board->noOfMovesFoward] = board->historyBack[board->noOfMovesBack - 1];
        board->noOfMovesFoward += 1;
        int moves = board->noOfMovesBack;
        initializeBoard(board);
        board->historyBack = realloc(board->historyBack, sizeof(Movement) * (moves - 1));
        board->historyBack = m;
        board->noOfMovesBack = 0;
        for (int i = 0; i < moves - 1; ++i) {
            makeMove(board, m[i]);
        }
    }
}

void goForward(Board *board) {
    if (canGoFoward(board)) {
        Movement *m = malloc(sizeof(Movement) * (board->noOfMovesFoward - 1));
        for (int i = 0; i < board->noOfMovesFoward-1; ++i) {
            m[i] = board->historyForward[i];
        }
        board->historyBack = realloc(board->historyBack, sizeof(Movement) * (board->noOfMovesBack + 1));
        board->historyBack[board->noOfMovesBack] = board->historyForward[board->noOfMovesFoward - 1];
        board->noOfMovesFoward -= 1;
        int moves = board->noOfMovesBack+1;
        Movement *historyRebuild = board->historyBack;
        initializeBoard(board);
        board->historyForward = realloc(board->historyForward, sizeof(Movement) * (moves - 1));
        board->historyForward = m;
        board->noOfMovesBack = 0;
        for (int i = 0; i < moves; ++i) {
            makeMove(board, historyRebuild[i]);
        }
    }
}

void removeHistoryFoward(Board *board){
    free(board->historyForward);
    board->historyForward = malloc(sizeof(Movement));
    board->noOfMovesFoward =0;
}

void make1Move(Board *board, Movement lastMove, int replace){
    int SIZE = board->size;
    int rowChange;
    int colChange;
    int x;
    int y;

    if (replace) {
        free(board->historyForward);
        board->historyForward = malloc(sizeof(Movement));
        board->noOfMovesFoward =0;
    }

    for(int i = 0; i < board->size; i++){
        for (int j = 0; j < board->size; j++) {
            if (board->state[i][j]->pieceType == HELPER) {
                board->state[i][j]->pieceType = VOID;
            }
        }
    }

    char opponent = (lastMove.pieceState == WHITE_PIECE) ? BLACK_PIECE : WHITE_PIECE;

    board->state[lastMove.x][lastMove.y]->pieceType = lastMove.pieceState;

    for(rowChange = -1; rowChange <= 1; rowChange++)
        for(colChange = -1; colChange <= 1; colChange++)
        {
            if(lastMove.x + rowChange < 0 || lastMove.x + rowChange >= SIZE ||
               lastMove.y + colChange < 0 || lastMove.y + colChange >= SIZE ||
               (rowChange == 0 && colChange == 0))
                continue;

            if(board->state[lastMove.x + rowChange][lastMove.y + colChange]->pieceType == opponent)
            {

                x = lastMove.x + rowChange;
                y = lastMove.y + colChange;

                for(;;)
                {
                    x += rowChange;
                    y += colChange;


                    if(x < 0 || x >= SIZE || y < 0 || y >= SIZE)
                        break;


                    if(board->state[x][y]->pieceType == VOID)
                        break;

                    if(board->state[x][y]->pieceType == lastMove.pieceState)
                    {
                        while(board->state[x-=rowChange][y-=colChange]->pieceType == opponent)
                            board->state[x][y]->pieceType = lastMove.pieceState;
                        break;
                    }
                }
            }
        }
    board->historyBack = realloc(board->historyBack, sizeof(Movement) * (board->noOfMovesBack + 1));
    board->historyBack[board->noOfMovesBack] = lastMove;
    board->noOfMovesBack +=1;
}



void makeMove(Board *board, Movement lastMove){
    char opponent = (lastMove.pieceState == WHITE_PIECE) ? BLACK_PIECE : WHITE_PIECE;

    int rowIndex , colIndex;
    Board tmp;
    initializeGame(&tmp);
    for (int k = 0; k < board->size; k++) {
        for (int l = 0; l < board->size; l++){
            tmp.state[k][l]->pieceType=board->state[k][l]->pieceType;
        }
    }

    if(lastMove.x<0 || lastMove.x>board->size-1 || lastMove.y<0 || lastMove.y>board->size-1)return;

    puts("inicio");
    printf("%d %d %d\n", lastMove.pieceState, lastMove.x, lastMove.y);

    board->historyBack = realloc(board->historyBack, sizeof(Movement) * (board->noOfMovesBack + 1));
    board->historyBack[board->noOfMovesBack] = lastMove;
    board->noOfMovesBack +=1;

    tmp.state[lastMove.x][lastMove.y]->pieceType=lastMove.pieceState;

    //move down
    Movement * moves;
    moves = malloc(sizeof(Movement));
    rowIndex = lastMove.x - 1;
    colIndex = lastMove.y;
    int counter=0;
    while(rowIndex > 0 && board->state[rowIndex][colIndex]->pieceType == opponent){
        moves = realloc(moves, sizeof(*moves) + sizeof(Movement));
        Movement m = (Movement){lastMove.pieceState, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex--;
    }
    if(rowIndex >= 0 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceState && counter > 0){
        puts("down");
        for(int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceState;
        }
    }


    //move up
    free(moves);
    moves = malloc(sizeof(Movement));
    rowIndex = lastMove.x+1;
    colIndex = lastMove.y;
    counter=0;
    while(rowIndex < board->size - 1 && board->state[rowIndex][colIndex]->pieceType == opponent){
        moves = realloc(moves, sizeof(*moves) + sizeof(Movement));
        Movement m = (Movement){lastMove.pieceState, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex++;
    }
    if(rowIndex <= board->size - 1 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceState && counter > 0){
        puts("up");
        for(int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceState;
        }
    }

    //move right
    free(moves);
    moves = malloc(sizeof(Movement));
    rowIndex = lastMove.x;
    colIndex = lastMove.y-1;
    counter = 0;
    while(colIndex > 0 && board->state[rowIndex][colIndex]->pieceType == opponent){
        moves = realloc(moves, sizeof(*moves) + sizeof(Movement));
        Movement m = (Movement){lastMove.pieceState, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        colIndex--;
    }
    if(colIndex >= 0 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceState && counter > 0){
        puts("right");
        for(int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceState;
        }
    }


    //move left
    free(moves);
    moves = malloc(sizeof(Movement));
    rowIndex = lastMove.x;
    colIndex = lastMove.y+1;
    counter = 0;
    while(colIndex < board->size - 1 && board->state[rowIndex][colIndex]->pieceType == opponent){
        moves = realloc(moves, sizeof(*moves) + sizeof(Movement));
        Movement m = (Movement){lastMove.pieceState, rowIndex, colIndex};
        moves[counter] =m;
        counter++;
        colIndex++;
    }
    if(colIndex <= board->size - 1 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceState && counter > 0){
        puts("left");
        for(int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceState;
        }
    }

    //move down right
    free(moves);
    moves = malloc(sizeof(Movement));
    rowIndex = lastMove.x - 1;
    colIndex = lastMove.y - 1;
    counter = 0;
    while(rowIndex > 0 && colIndex > 0 && board->state[rowIndex][colIndex]->pieceType == opponent){
        moves = realloc(moves, sizeof(*moves) + sizeof(Movement));
        Movement m = (Movement){lastMove.pieceState, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex--;
        colIndex--;
    }
    if(rowIndex >= 0 && colIndex >= 0 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceState && counter > 0){
        puts("down right");
        for(int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceState;
        }
    }

    //move up right
    free(moves);
    moves = malloc(sizeof(Movement));
    rowIndex = lastMove.x - 1;
    colIndex = lastMove.y + 1;
    counter = 0;
    while(rowIndex > 0 && colIndex < board->size - 1 && board->state[rowIndex][colIndex]->pieceType == opponent){
        moves = realloc(moves, sizeof(*moves) + sizeof(Movement));
        Movement m = (Movement){lastMove.pieceState, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex--;
        colIndex++;
    }
    if(rowIndex >= 0 && colIndex <= board->size - 1 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceState && counter > 0){
        puts("up right");
        for(int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceState;
        }
    }

    //move down left
    free(moves);
    moves = malloc(sizeof(Movement));
    rowIndex = lastMove.x + 1;
    colIndex = lastMove.y - 1;
    counter = 0;
    while(rowIndex < board->size - 1 && colIndex > 0 && board->state[rowIndex][colIndex]->pieceType == opponent){
        moves = realloc(moves, sizeof(*moves) + sizeof(Movement));
        Movement m = (Movement){lastMove.pieceState, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex++;
        colIndex--;
    }
    if(rowIndex <= board->size - 1 && colIndex >= 0 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceState && counter > 0){
        puts("down left");
        for(int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceState;
        }
    }

    //move up left
    free(moves);
    moves = malloc(sizeof(Movement));
    puts("antes restas");
    rowIndex = lastMove.x + 1;
    colIndex = lastMove.y + 1;
    puts("despues restas");
    counter = 0;
    while(rowIndex < board->size - 1 && colIndex < board->size - 1 && board->state[rowIndex][colIndex]->pieceType == opponent){
        moves = realloc(moves, sizeof(*moves) + sizeof(Movement));
        Movement m = (Movement){lastMove.pieceState, rowIndex, colIndex};
        moves[counter] = m;
        counter++;
        rowIndex++;
        colIndex++;
    }
    puts("paso while");
    if(rowIndex <= board->size - 1 && colIndex <= board->size - 1 && board->state[rowIndex][colIndex]->pieceType == lastMove.pieceState && counter > 0){
        puts("up left");
        for(int i = 0; i < counter; i++) {
            tmp.state[moves[i].x][moves[i].y]->pieceType = lastMove.pieceState;
        }
    }



    for (int k = 0; k < board->size; k++) {
        for (int l = 0; l < board->size; l++){
            board->state[k][l]->pieceType=tmp.state[k][l]->pieceType;
        }
    }
    puts("fin");
}








