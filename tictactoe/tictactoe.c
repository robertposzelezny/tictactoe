#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tictactoe.h"

void initGame(Game* game) {
    memset(game->board, ' ', sizeof(game->board));
    game->player1_score = 0;
    game->player2_score = 0;
    game->draws = 0;
}

void resetBoard(Game* game) {
    memset(game->board, ' ', sizeof(game->board));
}

void displayBoard(const Game* game) {
    int i;
    int j;
    printf("\n");
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            printf(" %c ", game->board[i][j]);
            if (j < BOARD_SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < BOARD_SIZE - 1) printf("---+---+---\n");
    }
    printf("\n");
}

int makeMove(Game* game, int row, int col, char symbol) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || game->board[row][col] != ' ')
        return 0;
    game->board[row][col] = symbol;
    return 1;
}

int checkGameState(const Game* game) {
    int moves;
    int i;
    int j;
    for (i = 0; i < BOARD_SIZE; i++) {
        if (game->board[i][0] != ' ' &&
            game->board[i][0] == game->board[i][1] &&
            game->board[i][1] == game->board[i][2]) {
            return (game->board[i][0] == game->player1_symbol) ? 1 : 2;
        }
        if (game->board[0][i] != ' ' &&
            game->board[0][i] == game->board[1][i] &&
            game->board[1][i] == game->board[2][i]) {
            return (game->board[0][i] == game->player1_symbol) ? 1 : 2;
        }
    }
    if (game->board[0][0] != ' ' &&
        game->board[0][0] == game->board[1][1] &&
        game->board[1][1] == game->board[2][2]) {
        return (game->board[0][0] == game->player1_symbol) ? 1 : 2;
    }
    if (game->board[0][2] != ' ' &&
        game->board[0][2] == game->board[1][1] &&
        game->board[1][1] == game->board[2][0]) {
        return (game->board[0][2] == game->player1_symbol) ? 1 : 2;
    }

    moves = 0;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] != ' ') moves++;
        }
    }
    if (moves == BOARD_SIZE * BOARD_SIZE)
        return 0;
    return -1;
}

void displayStats(const Game* game) {
    printf("Statistics:\n");
    printf("%s (symbol: %c): %d wins\n", game->player1_name, game->player1_symbol, game->player1_score);
    printf("%s (symbol: %c): %d wins\n", game->player2_name, game->player2_symbol, game->player2_score);
    printf("Draws: %d\n", game->draws);
}

char selectGameMode() {
    char choice;
    printf("Select game mode: \n");
    printf("1 - play against other player\n");
    printf("2 - play against random moves comp\n");
    printf("3 - hard mode\n");
    printf("4 - quit game\n");
    while (1) {
        scanf_s(" %c", &choice, sizeof(choice));
        if (choice == '1' || choice == '2' || choice == '3' || choice == '4')
            return choice;
        else
            printf("incorrect selection!\n");
    }
}

void checkGameBoard(const Game* game, int* emptySpaces, int* size) {
    int index = 0;
    int i;
    int j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] == ' ') {
                emptySpaces[index++] = i * BOARD_SIZE + j;
            }
        }
    }
    *size = index;
}
void makeRandomMove(Game* game) {
    int emptySpaces[BOARD_SIZE * BOARD_SIZE]; 
    int size = 0;
    int randomIndex;
    int position;
    int row;
    int col;
    checkGameBoard(game, emptySpaces, &size);
    if (size == 0) {
        printf("Board is full!\n");
        return;
    }
    srand(time(NULL));
    randomIndex = rand() % size;
    position = emptySpaces[randomIndex];
    row = position / BOARD_SIZE;
    col = position % BOARD_SIZE;

    game->board[row][col] = game->player2_symbol;
    printf("computer made a move on position: (%d, %d)\n", row, col);
}

int minimax(Game* game, int depth, int isMaximizing) {
    int state = checkGameState(game);
    int score;
    int bestScore;
    int i;
    int j;
    if (state == 1) return -10 + depth;
    if (state == 2) return 10 - depth;
    if (state == 0) return 0;

    if (isMaximizing) {
        bestScore = -1000;
        for (i = 0; i < BOARD_SIZE; i++) {
            for (j = 0; j < BOARD_SIZE; j++) {
                if (game->board[i][j] == ' ') {
                    game->board[i][j] = game->player2_symbol;
                    score = minimax(game, depth + 1, 0);
                    game->board[i][j] = ' ';
                    bestScore = score > bestScore ? score : bestScore;
                }
            }
        }
        return bestScore;
    }
    else {
        bestScore = 1000;
        for (i = 0; i < BOARD_SIZE; i++) {
            for (j = 0; j < BOARD_SIZE; j++) {
                if (game->board[i][j] == ' ') {
                    game->board[i][j] = game->player1_symbol;
                    score = minimax(game, depth + 1, 1);
                    game->board[i][j] = ' ';
                    bestScore = score < bestScore ? score : bestScore;
                }
            }
        }
        return bestScore;
    }
}

void makeOptimalMove(Game* game) {
    int bestScore = -1000;
    int moveRow = -1, moveCol = -1;
    int score;
    int i;
    int j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] == ' ') {
                game->board[i][j] = game->player2_symbol;
                score = minimax(game, 0, 0);
                game->board[i][j] = ' ';
                if (score > bestScore) {
                    bestScore = score;
                    moveRow = i;
                    moveCol = j;
                }
            }
        }
    }

    if (moveRow != -1 && moveCol != -1) {
        game->board[moveRow][moveCol] = game->player2_symbol;
        printf("Computer made a move on position: (%d, %d)\n", moveRow, moveCol);
    }
}

