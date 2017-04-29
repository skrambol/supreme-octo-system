#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int row;
    int col;
} mov;

int puzzles_count;
FILE *in, *out;

int **board;
int dimension;
int chancellor_count;

void initializeBoard();
void printBoard(int **, char *);
int backtrack();
mov initializeMove(int ,int);
int isBefore(mov, mov);
int checkMove(mov, int);
int doMove(mov, int);

void main() {
    int i, j;

    in = fopen("project.in", "r");
    out = fopen("solutions.out", "w");

    fscanf(in, "%d \n", &puzzles_count);
    printf("[LOG] puzzles_count: %d\n", puzzles_count);

    for (i=0; i<puzzles_count; i++) {
        printf("\n[LOG] PUZZLE-%d:\n", i);
        fscanf(in, "%d \n", &dimension);
        printf("[LOG] dimension: %d\n", dimension);
        initializeBoard();
        if (!backtrack()) {
            printf("[LOG] there are no possible solutions\n");
        }

        for(j=0; j<dimension; j++) {
            free(board[j]);
        }
        free(board);
    }

    fclose(in);
    fclose(out);
}

void initializeBoard() {
    int i, j;

    chancellor_count = 0;
    board = (int **) malloc (sizeof(int *)*dimension);

    for (i=0; i<dimension; i++) {
        board[i] = (int *) malloc (sizeof(int)*dimension);
        for (j=0; j<dimension; j++) {
            fscanf(in, "%d", &board[i][j]);
            if (board[i][j]) chancellor_count++;
        }
    }

    printBoard(board, "board");
}

void printBoard(int **board, char *name) {
    int i, j;

    printf("[LOG] %s: \n", name);

    for (i=0; i<dimension; i++) {
        for (j=0; j<dimension; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

int backtrack() {
    int log_move, log;
    int i, j, start, nth_move, solutions_count;
    int tos[dimension+2];
    mov candidate, **solutions;

    printf("[LOG] backtracking...\n");

    if (chancellor_count > dimension) {
        return 0;
    }

    if (chancellor_count == dimension) {
        int board_temp[dimension][dimension];

        for (i=0; i<dimension; i++) {
            for (j=0; j<dimension; j++) {
                board_temp[i][j] = board[i][j];
                board[i][j] = 0;
            }
        }

        for (i=0; i<dimension; i++) {
            for (j=0; j<dimension; j++) {
                if (board_temp[i][j]) {
                    mov move = initializeMove(i, j);
                    if(checkMove(move, 1)) {
                        board[i][j] = 1;
                    }
                    else {
                        return 0;
                    }
                }

            }
        }
    }

    solutions = (mov **) malloc (sizeof(mov *)*(dimension+2));
    for(i=0; i<dimension+2; i++) {
        solutions[i] = (mov *) malloc (sizeof(mov )*(dimension*dimension+2));
    }

    start = chancellor_count;
    nth_move = start;
    log_move = 0;
    log = 0;
    tos[start] = 1;
    solutions_count = 0;

    while(tos[start]>0) {
        if (tos[nth_move]>0) {
            nth_move++;
            tos[nth_move] = 0;

            if (nth_move==dimension+1) {
                candidate = solutions[nth_move-1][tos[nth_move-1]];
                doMove(candidate, 1);

                fprintf(out, "%dth solution\n", ++solutions_count);

                for (i=0; i<dimension; i++) {
                    for (j=0; j<dimension; j++) {
                        fprintf(out, "%d ", board[i][j]);
                        if (log) printf("%d ", board[i][j]);
                    }
                    if (log) printf("\n");
                    fprintf(out, "\n");
                }

                if (log)
                    for(i=start+1; i<=dimension; i++)
                        printf("[LOG] steps: %d, %d\n", solutions[i][tos[i]].row, solutions[i][tos[i]].col);
            }

            else {
                if (nth_move > 1) {
                    candidate = solutions[nth_move-1][tos[nth_move-1]];
                    doMove(candidate, 1);
                    if (log_move) printf("[LOG] %d-th stack; tos @ %d; move(%d, %d)\n", nth_move-1, tos[nth_move-1], candidate.row, candidate.col);
                }

                for (i=dimension-1; i>-1; i--) {
                    for (j=dimension-1; j>-1; j--) {
                        if (!board[i][j]) {
                            mov move = initializeMove(i,j);
                            if (checkMove(move, 0) && (nth_move == 1 || isBefore(candidate, move)) ) {
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
            if (!--nth_move) break;

            candidate = solutions[nth_move][tos[nth_move]];
            doMove(candidate, 0);
            if (log_move) printf("[LOG] %d-th stack; tos @ %d; undo(%d, %d)\n", nth_move, tos[nth_move], candidate.row, candidate.col);

            tos[nth_move]--;
        }
    }

    for(i=0; i<dimension+2; i++) {
        free(solutions[i]);
    }
    free(solutions);

    printf("[LOG] backtracked with %d solutions...\n", solutions_count);
    return solutions_count;
}

mov initializeMove(int row, int col) {
    mov move;

    move.row = row;
    move.col = col;

    return move;
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

    return 1;
}

int isBefore(mov prev, mov new) {
    return (
        prev.row*dimension + prev.col <
        new.row*dimension + new.col
    );
}

int doMove(mov move, int what_move) {
    if (move.row < dimension && move.row > -1 && move.col < dimension && move.col > -1) {
        board[move.row][move.col] = what_move;

        what_move ? chancellor_count++ : chancellor_count--;
        return 1;
    }

    return 0;
}
