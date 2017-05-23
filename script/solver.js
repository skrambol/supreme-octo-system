'use strict';

var REMOVE_CHANCELLOR   = 0;
var INIT_CHANCELLOR     = 1;
var PLACE_CHANCELLOR    = 2;

var solver = {

    init : function(chancellor) {
        return solver.backtrack(chancellor);
    },

    backtrack : function(chancellor) {
        var log, log_move;
        var i, j, dimension = chancellor.board.length,
            start, nth_move, solutions_count, tos = [];
        var move, move_candidate = {row : -1, col : -1}, move_solutions = [];
        var board_solutions = [];
        console.log("[LOG] Backtracking...");

        console.table(chancellor.board);

        start = chancellor.count;
        nth_move = start;
        log = log_move = false;
        solver.initTos(tos, dimension);
        solver.initMoveSolutions(move_solutions, dimension);
        tos[start] = 1;
        solutions_count = 0;

        if (chancellor.count > dimension) {
            return 0;
        }

        if (chancellor.count > 0) {
            if (log) console.log("[LOG] there is an initial chancellor");
            var board_temp = [], nth_move_temp = 0;

            chancellor.count = 0;

            for (i = 0; i < dimension; i++) {
                if (log) console.log(chancellor.board[i]);
                board_temp.push([]);
                for (j = 0; j < dimension; j++) {
                    board_temp[i].push(chancellor.board[i][j]);
                    chancellor.board[i][j] = 0;
                }
            }

            if (log) console.log(board_temp);

            for (i = 0; i < dimension; i++) {
                for(j = 0; j < dimension; j++) {
                    if(board_temp[i][j] > 0) {
                        move = solver.initMove(i, j);
                        if(solver.checkMove(chancellor.board, move, log)) {
                            nth_move_temp++;
                            tos[nth_move_temp] = 1;
                            solver.doMove(chancellor.board, chancellor.count, move, dimension, INIT_CHANCELLOR);
                            move_solutions[nth_move_temp][1] = move;
                            if (log_move) console.log("[LOG] " + nth_move_temp + "-th stack; tos @ " + tos[nth_move_temp] + "; init(" + move.row + ", " + move.col + ")");
                        } else {
                            if (log) console.log("Incomplete board initialization, returning...");
                            return 0;
                        }
                    }
                }
            }
        }

        console.log(tos[start]);
        while (tos[start] > 0) {
            if (tos[nth_move] > 0) {
                nth_move++;
                tos[nth_move] = 0;

                if (log) console.log("[LOG] now at " + nth_move + "-th stack");

                if (nth_move == dimension + 1) {
                    if(chancellor.count < dimension) {
                        move_candidate = move_solutions[nth_move - 1][tos[nth_move - 1]];
                        solver.doMove(chancellor.board, chancellor.count, move_candidate, dimension, PLACE_CHANCELLOR);

                        solutions_count++;
                        board_solutions.push(JSON.parse(JSON.stringify(chancellor.board)));
                        if (log) console.log("[Solution LOG] solutions_count: " + solutions_count);

                        if (log) {
                            for(i=start+1; i<=dimension; i++)         
                                console.log("[LOG] steps: " + move_solutions[i][tos[i]].row + ", " + move_solutions[i][tos[i]].col + "");
                            solver.printBoard(chancellor.board);
                        }
                    }
                } else {
                    if (nth_move >= start + 1) {
                        move_candidate = move_solutions[nth_move - 1][tos[nth_move - 1]];
                        if (nth_move > start + 1) solver.doMove(chancellor.board, chancellor.count, move_candidate, dimension, PLACE_CHANCELLOR);
                        if (log_move) console.log("[LOG] " + nth_move + "-th stack; tos @ " + tos[nth_move] + "; move(" + move_candidate.row + ", " + move_candidate.col + ")");
                    }

                    for (i = dimension - 1; i > -1; i--) {
                        for (j = dimension - 1; j > -1; j--) {
                            if (chancellor.board[i][j] == 0) {
                                move = solver.initMove(i, j);
                                if (solver.checkMove(chancellor.board, move, log) && (nth_move == 1 || solver.isBefore(chancellor.board, dimension, move_candidate, move))) {
                                    tos[nth_move]++;
                                    move_solutions[nth_move][tos[nth_move]] = move;
                                    if (log_move) console.log("[LOG] " + nth_move + "-th stack; tos @ " + tos[nth_move] + "; add(" + move.row + ", " + move.col + ")");
                                }
                            }
                        }
                    }
                }
            } else {
                if(--nth_move == 0) break;

                move_candidate = move_solutions[nth_move][tos[nth_move]];
                solver.doMove(chancellor.board, chancellor.count, move_candidate, dimension, REMOVE_CHANCELLOR);
                if (log_move) console.log("[LOG] " + nth_move + "-th stack; tos @ " + tos[nth_move] + "; undo(" + move_candidate.row + ", " + move_candidate.col + ")");

                tos[nth_move]--;
            }
        }
        console.log("[LOG] Backtracked with " + solutions_count + " solution/s!");
        return { count : solutions_count, boards : board_solutions};
    },

    initTos : function(tos, dimension) {
        var i;
        for (i = 0; i < dimension + 2; i++) {
            tos.push(-1);
        }
    },

    initMoveSolutions : function(move_solutions, dimension) {
        var i, j;
        for (i = 0; i < dimension + 2; i++) {
            move_solutions.push([]);
            for (j = 0; j < dimension * dimension + 2; j++) {
                move_solutions[i].push({row : -1, col : -1});
            }
        }
    },

    initMove : function(i, j) {
        return {row : i, col : j};
    },

    checkMove : function(board, move, log) {
        var rookMove = chancellor.checkRookMove(board, move.row, move.col);
        var knighMove = chancellor.checkKnightMove(board, move.row, move.col)
        if (log) console.log("[LOG] move at: " + move.row + ", " + move.col + ", rookMove: " + rookMove + ", knighMove: " + knighMove);
        return (rookMove && knighMove);
    },

    doMove : function(board, count, move, dimension, what_move) {
        if ((move.row < dimension && move.row > -1) && (move.col < dimension && move.col > -1)) {
            board[move.row][move.col] = what_move;

            what_move > 0 ? count++ : count--;
            return 1;
        }
        return 0;
    },

    isBefore : function(board, dimension, move_prev, move_new) {
        return (
            board[move_prev.row][move_prev.col] == INIT_CHANCELLOR || (move_prev.row * dimension + move_prev.col < move_new.row * dimension + move_new.col)
        );
    },

    printBoard : function(board) {
        var dimension = board.length, i, j;
        var space = " ";

        for (i = 0; i < dimension; i++) {
            var str = "";
            for (j = 0; j < dimension; j++) {
                str += board[i][j] + " ";
            }
            console.log(str + space);
            space += " ";
        }
    }
}
