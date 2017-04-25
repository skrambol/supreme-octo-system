#include <stdio.h>
#include <stdlib.h>
#define CHANCELLOR 1

int NO_OF_PUZZLES;
FILE *FP;

int **board;
int *board_row;
int *board_col;
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
    if (board_row[move_row]) {
        return 0;
    }

    //check col
    if (board_col[move_col]) {
        return 0;
    }

    //check L
    if (dimension < 3) {
        return 1;
    }

    //implement legit knight move checking
}

void initializeBoard() {
    int i, j;

    board = (int **) malloc (sizeof(int *)*dimension);
    board_row = (int *) malloc (sizeof(int *)*dimension);
    board_col = (int *) malloc (sizeof(int *)*dimension);

    for (i=0; i<dimension; i++) {
        board[i] = (int *) malloc (sizeof(int)*dimension);
        for (j=0; j<dimension; j++) {
            fscanf(FP, "%d", &board[i][j]);
        }
    }

    printBoard();
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
    }

    fclose(FP);
}
