#include <stdio.h>
#include <stdlib.h>
#define CHANCELLOR 1

int NO_OF_PUZZLES;
FILE *FP;

int **board;
int dimension;
int count_chancellor;

void printBoard() {
    int i, j;

    printf("[LOG] board: \n");

    for (i=0; i<dimension; i++) {
        for (j=0; j<dimension; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

int checkMove(int move_row, int move_col) {
    int i, j;

    printf("[LOG] move at: %d, %d\n", move_row, move_col);

    //check rook-move
    for (i=0; i<dimension; i++) {
        if (board[move_row][i] || board[i][move_col]) {
            printf("[LOG] invalid rook move\n");
            return 0;
        }
    }

    //check knight-move
    if (dimension < 3) {
        count_chancellor++;
        return 1;
    }


    for (i=1; i<=2; i++) {
        for(j=2; j>=1; j--) {
            if (i==j) break;
            if (move_row+i < dimension) {
                if (move_col+j < dimension && board[move_row+i][move_col+j]) {
                    printf("[LOG] invalid knight move at +%d, +%d\n",i ,j);
                    return 0;
                }
                if (move_col-j > -1 && board[move_row+i][move_col-j]) {
                    printf("[LOG] invalid knight move at +%d, -%d\n",i ,j);
                    return 0;
                }
            }
            if (move_row-i > -1) {
                if (move_col+j < dimension && board[move_row-i][move_col+j]) {
                    printf("[LOG] invalid knight move at -%d, +%d\n",i ,j);
                    return 0;
                }
                if (move_col-j > -1 && board[move_row-i][move_col-j]) {
                    printf("[LOG] invalid knight move at -%d, -%d\n",i ,j);
                    return 0;
                }
            }
        }
    }

    count_chancellor++;
    return 1;
}

void initializeBoard() {
    int i, j;

    count_chancellor = 0;
    board = (int **) malloc (sizeof(int *)*dimension);

    for (i=0; i<dimension; i++) {
        board[i] = (int *) malloc (sizeof(int)*dimension);
        for (j=0; j<dimension; j++) {
            fscanf(FP, "%d", &board[i][j]);
            if (board[i][j]) count_chancellor++;
        }
    }

    printBoard();
}

int backtrack() {
    int i, j;
    printf("[LOG] backtracking...\n");

    if (count_chancellor > dimension) return 0;

    for (i=0; i<dimension; i++) {
        for (j=0; j<dimension; j++) {
            if (!board[i][j]) {
                board[i][j] = checkMove(i,j);
                printBoard();
            }
        }
    }

    printf("[LOG] backtracked...\n");
    return 1;
}

void initializeState() {
    int i;

    FP = fopen("project.in", "r");

    fscanf(FP, "%d \n", &NO_OF_PUZZLES);
    printf("[LOG] NO_OF_PUZZLES: %d\n", NO_OF_PUZZLES);

    fscanf(FP, "%d \n", &dimension);
    printf("[LOG] dimension: %d\n", dimension);

    for (i=0; i<NO_OF_PUZZLES; i++) {
        initializeBoard();
        if (!backtrack()) {
            printf("[LOG] there are no possible solutions");
        }
    }

    fclose(FP);
}
