#include <stdio.h>
#include <stdlib.h>
#define CHANCELLOR 1

typedef struct node {
    int row;
    int col;
} mov;

int NO_OF_PUZZLES;
FILE *FP;

int **board;
int **board_init;
int dimension;
int chancellor_count;

void printBoard(int **board, char *name, int log) {
    if (!log) return;

    int i, j;

    printf("[LOG] %s: \n", name);

    for (i=0; i<dimension; i++) {
        for (j=0; j<dimension; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

mov initializeMove(int row, int col) {
    mov move;

    move.row = row;
    move.col = col;

    return move;
}

int doMove(mov move, int what_move) {
    if (move.row < dimension && move.row > -1 && move.col < dimension && move.col > -1) {
        board[move.row][move.col] = what_move;
        return 1;
    }

    return 0;
}

int checkMove(mov move, int log) {
    int i, j;

    if (log) printf("[LOG] move at: %d, %d\n", move.row, move.col);

    //check rook-move
    for (i=0; i<dimension; i++) {
        if (board[move.row][i] || board[i][move.col]) {
            if (log) printf("[LOG] invalid rook move\n");
            return 0;
        }
    }

    //check knight-move
    if (dimension < 3) {
        chancellor_count++;
        return 1;
    }

    for (i=1; i<=2; i++) {
        for(j=2; j>=1; j--) {
            if (i==j) continue;
            if (move.row+i < dimension) {
                if (move.col+j < dimension && board[move.row+i][move.col+j]) {
                    if (log) printf("[LOG] invalid knight move at +%d, +%d\n",i ,j);
                    return 0;
                }
                if (move.col-j > -1 && board[move.row+i][move.col-j]) {
                    if (log) printf("[LOG] invalid knight move at +%d, -%d\n",i ,j);
                    return 0;
                }
            }
            if (move.row-i > -1) {
                if (move.col+j < dimension && board[move.row-i][move.col+j]) {
                    if (log) printf("[LOG] invalid knight move at -%d, +%d\n",i ,j);
                    return 0;
                }
                if (move.col-j > -1 && board[move.row-i][move.col-j]) {
                    if (log) printf("[LOG] invalid knight move at -%d, -%d\n",i ,j);
                    return 0;
                }
            }
        }
    }

    chancellor_count++;
    return 1;
}

void initializeBoard() {
    int i, j;

    chancellor_count = 0;
    board = (int **) malloc (sizeof(int *)*dimension);
    board_init = (int **) malloc (sizeof(int *)*dimension);

    for (i=0; i<dimension; i++) {
        board[i] = (int *) malloc (sizeof(int)*dimension);
        board_init[i] = (int *) malloc (sizeof(int)*dimension);
        for (j=0; j<dimension; j++) {
            fscanf(FP, "%d", &board[i][j]);
            board_init[i][j] = board[i][j];
            if (board[i][j]) chancellor_count++;
        }
    }

    printBoard(board, "board AND board_init", 1);
}

int backtrack() {
    int log_move=0, log=1;
    int i, j, start, nth_move, solutions_count=0;
    int tos[dimension+2];
    mov candidate, **solutions;
    FILE *out;

    printf("[LOG] backtracking...\n");

    solutions = (mov **) malloc (sizeof(mov *)*(dimension+2));

    for(i=0; i<dimension+2; i++) {
        solutions[i] = (mov *) malloc (sizeof(mov )*(dimension*dimension+2));
    }

    out = fopen("solutions.out", "w");

    start = chancellor_count;
    nth_move = start;
    tos[start] = 1;
    while(tos[start]>0) {
        if (tos[nth_move]>0) {
            nth_move++;
            tos[nth_move] = 0;

            if (nth_move==dimension+1) {
                candidate = solutions[nth_move-1][tos[nth_move-1]];
                doMove(candidate, 1);
                //printBoard(board, "solution", 1);

                for (i=0; i<dimension; i++) {
                    for (j=0; j<dimension; j++) {
                        fprintf(out, "%d ", board[i][j]);
                    }
                    fprintf(out, "\n");
                }

                fprintf(out, "-\n");

                for(i=start+1; i<=dimension; i++) {
                    printf("[LOG] steps: %d, %d\n", solutions[i][tos[i]].row, solutions[i][tos[i]].col);
                }

                solutions_count++;

            }

            else if (nth_move==1) { //initialize first move
                for (i=dimension-1; i>-1; i--) {
                    for (j=dimension-1; j>-1; j--) {
                        if (!board[i][j]) {
                            mov move = initializeMove(i,j);
                            if (checkMove(move, 0)) {
                                tos[nth_move]++;
                                solutions[nth_move][tos[nth_move]] = move;
                                if (log_move) printf("[LOG] %d-th stack; tos @ %d; init(%d, %d)\n", nth_move, tos[nth_move], move.row, move.col);
                            }
                        }
                    }
                }
            }

            else {
                candidate = solutions[nth_move-1][tos[nth_move-1]];
                doMove(candidate, 1);
                if (log_move) printf("[LOG] %d-th stack; tos @ %d; move(%d, %d)\n", nth_move-1, tos[nth_move-1], candidate.row, candidate.col);

                for (i=dimension-1; i>-1; i--) {
                    for (j=dimension-1; j>-1; j--) {
                        if (!board[i][j]) {
                            mov move = initializeMove(i,j);
                            if (checkMove(move, 0)) {
                                tos[nth_move]++;
                                solutions[nth_move][tos[nth_move]] = move;
                                if (log_move) printf("[LOG] %d-th stack; tos @ %d; add(%d, %d)\n", nth_move, tos[nth_move], move.row, move.col);
                            }
                        }
                    }
                }
            }
        }

        else {
            nth_move--;
            if (nth_move==0) break;

            candidate = solutions[nth_move][tos[nth_move]];
            doMove(candidate, 0);
            if (log_move) printf("[LOG] %d-th stack; tos @ %d; undo(%d, %d)\n", nth_move, tos[nth_move], candidate.row, candidate.col);

            tos[nth_move]--;

        }
    }

    free(solutions);
    fclose(out);

    printf("[LOG] backtracked with %d solutions...\n", solutions_count);
    return solutions_count;
}

void main() {
    int i;

    FP = fopen("project.in", "r");

    fscanf(FP, "%d \n", &NO_OF_PUZZLES);
    printf("[LOG] NO_OF_PUZZLES: %d\n", NO_OF_PUZZLES);


    for (i=0; i<NO_OF_PUZZLES; i++) {
        fscanf(FP, "%d \n", &dimension);
        printf("[LOG] dimension: %d\n", dimension);
        initializeBoard();
        if (!backtrack()) {
            printf("[LOG] there are no possible solutions\n");
        }

        for(i=0; i<dimension; i++)
            free(board[i]);
        free(board);
    }

    fclose(FP);
}
