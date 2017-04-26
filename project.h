#include <stdio.h>
#include <stdlib.h>
#define CHANCELLOR 1

int NO_OF_PUZZLES;
FILE *FP;

int **board;
int dimension;

void printBoard() {
    int i, j;

    printf("[LOG] board: \n");

    for (i=0; i<dimension; i++) {
        for (j=0; j<dimension; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}

int checkMove(int move_row, int move_col) {
    int i, j;

    //check rook-move
    for (i=0; i<dimension; i++) {
        if (board[move_row][i]) return 0;
        if (board[i][move_col]) return 0;
    }

    //check knight-move
    if (dimension < 3) {
        return 1;
    }

    for (i=1; i<=2; i++) {
        for(j=2; j>=1; j--) {
            if (move_row+i < dimension) {
                if (move_col+j < dimension && board[move_row+i][move_col+j]) {
                    return 0;
                }
                if (move_col-j > dimension && board[move_row+i][move_col-j]) {
                    return 0;
                }
            }
            else if (move_row-i > dimension) {
                if (move_col+j < dimension && board[move_row-i][move_col+j]) {
                    return 0;
                }
                if (move_col-j > dimension && board[move_row-i][move_col-j]) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

void initializeBoard() {
    int i, j;

    board = (int **) malloc (sizeof(int *)*dimension);

    for (i=0; i<dimension; i++) {
        board[i] = (int *) malloc (sizeof(int)*dimension);
        for (j=0; j<dimension; j++) {
            fscanf(FP, "%d", &board[i][j]);
        }
    }

    printBoard();
}

void backtrack() {
    printf("backtracking...");
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
        backtrack();
    }

    fclose(FP);
}
