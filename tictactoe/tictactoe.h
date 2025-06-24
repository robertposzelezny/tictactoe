#ifndef tictactoe
#define tictactoe

#define BOARD_SIZE 3

typedef struct {
    char board[BOARD_SIZE][BOARD_SIZE];
    char player1_name[50];
    char player2_name[50];
    char player1_symbol;
    char player2_symbol;
    int player1_score;
    int player2_score;
    int draws;
} Game;

char selectGameMode();
void initGame(Game* game);
void resetBoard(Game* game);
void displayBoard(const Game* game);
int makeMove(Game* game, int row, int col, char symbol);
int checkGameState(const Game* game);
void displayStats(const Game* game);
void makeRandomMove(Game* game);
void checkGameBoard(const Game*, int*, int*);
void makeOptimalMove(Game* game);

#endif
