#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tictactoe.h"

int main() {
    Game game;
    char choice;
    char gameMode;
    int currentPlayer, state;
    int row, col;
    while (1) {
        gameMode = selectGameMode();
        if (gameMode == '4') return 0;
        initGame(&game);
        printf("Enter name of Player 1: ");
        scanf_s("%49s", game.player1_name, sizeof(game.player1_name));
        if (gameMode == '1') {
            printf("Enter name of Player 2: ");
            scanf_s("%49s", game.player2_name, sizeof(game.player2_name));
        }
        else { strcpy_s(game.player2_name, sizeof(game.player2_name), "Computer"); }
        printf("Player 1, choose your symbol (X/O): ");
        scanf_s(" %c", &game.player1_symbol, sizeof(game.player1_symbol));
        game.player2_symbol = (game.player1_symbol == 'X' || game.player1_symbol == 'x') ? 'O' : 'X';

        srand(time(NULL));
        currentPlayer = rand() % 2;

        while (1) {
            resetBoard(&game);
            state = -1;

            while (state == -1) {
                displayBoard(&game);
                if (gameMode == '1') {
                    printf("%s's turn (%c). Enter row and column (0-2):\n",
                        currentPlayer == 0 ? game.player1_name : game.player2_name,
                        currentPlayer == 0 ? game.player1_symbol : game.player2_symbol);
                    scanf_s("%d %d", &row, &col);

                    if (!makeMove(&game, row, col, currentPlayer == 0 ? game.player1_symbol : game.player2_symbol)) {
                        printf("Invalid move. Try again.\n");
                        continue;
                    }
                }
                else {
                    if (currentPlayer == 0) {
                        printf("%s's turn (%c). Enter row and column (0-2):\n",
                            game.player1_name,
                            game.player1_symbol);
                        scanf_s("%d %d", &row, &col);

                        if (!makeMove(&game, row, col, currentPlayer == 0 ? game.player1_symbol : game.player2_symbol)) {
                            printf("Invalid move. Try again.\n");
                            continue;
                        }
                    }
                    else {
                        printf("%s's turn (%c).", game.player2_name, game.player2_symbol);
                        if (gameMode == '2') {
                            makeRandomMove(&game);
                        }
                        if (gameMode == '3') {
                            makeOptimalMove(&game);
                        }
                    }
                }

                state = checkGameState(&game);
                if (state == -1) currentPlayer = 1 - currentPlayer;
            }

            displayBoard(&game);
            if (state == 0) {
                printf("It's a draw!\n");
                game.draws++;
            }
            else {
                printf("%s wins!\n", state == 1 ? game.player1_name : game.player2_name);
                if (state == 1) game.player1_score++;
                else game.player2_score++;
            }

            displayStats(&game);
            printf("Play again? (y/n): ");
            scanf_s(" %c", &choice, sizeof(choice));

            if (choice == 'n' || choice == 'N') {
                break;
            }
        }
    }
    

    return 0;
}
