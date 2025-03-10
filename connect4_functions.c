#include "connect4.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_DEPTH 3
#define WIN_SCORE 1000000
#define INF 10000000

// Checks whether 'move' is valid given the current state of 'board'.
// It returns 1 when the move is valid, otherwise 0.
int valid_move(int board[][COLUMNS], int move) {
	if (move < 0 || move >= COLUMNS) return 0;
	if (board[0][move]) return 0;
	return 1;
}


// Update the board with a move for a specific player.
// Returns 0 in case of failure (the move is not a valid
// one); otherwise it returns 1. 
int add_move_board(int board[][COLUMNS], int move, int player) {
	if (!valid_move(board, move)) return 0;
	int i;
	for (i = ROWS - 1; i >= 0; i--) {
		if (!board[i][move]) {
			board[i][move] = player;
			return 1;
		}
	}
	return 0;
}


// It asks the user to pick a column to play, and repeats
// this until a valid one is chosen.
// The user should enter a number between 1 and COLUMNS (1-indexed).
// It returns the column in the board array, so a value between 
// 0 and COLUMNS-1 (0-indexed)
int player_move(int board[][COLUMNS]) {
	int n;
	printf("Which column do you want to play? ");
	scanf("%d", &n);
	while (!valid_move(board, n - 1)) {
		printf("Which column do you want to play? ");
		scanf("%d", &n);
	}
	return n - 1;
}

int getrow(int board[][COLUMNS], int col) {
	int row;
	for (row = ROWS - 1; row >= 0; row--) {
		if (!board[row][col]) return row;
	}
	return -1;
}

int checkWin(int board[][COLUMNS], int player) {
	// new method for better clarity
	int row, col;
	for (row = 0; row < ROWS; row++) {
        	for (col = 0; col <= COLUMNS - 4; col++) {
            		if (board[row][col] == player && 
                	board[row][col+1] == player && 
                	board[row][col+2] == player && 
                	board[row][col+3] == player) {
                		return 1;
            		}
        	}	
    	}
	for (row = 0; row <= ROWS - 4; row++) {
        	for (col = 0; col < COLUMNS; col++) {
            		if (board[row][col] == player && 
                	board[row+1][col] == player && 
                	board[row+2][col] == player && 
                	board[row+3][col] == player) {
                		return 1;
            		}
        	}
    	}
	for (row = 0; row <= ROWS - 4; row++) {
        	for (col = 0; col <= COLUMNS - 4; col++) {
            		if (board[row][col] == player &&
                	board[row+1][col+1] == player &&
                	board[row+2][col+2] == player &&
                	board[row+3][col+3] == player) {
                		return 1;
            		}
        	}
    	}
	for (row = 3; row < ROWS; row++) {
        	for (col = 0; col <= COLUMNS - 4; col++) {
            		if (board[row][col] == player &&
                	board[row-1][col+1] == player &&
                	board[row-2][col+2] == player &&
                	board[row-3][col+3] == player) {
                		return 1;
            		}
        	}	
    	}
	return 0;
}

int isBoardFull(int board[][COLUMNS]) {
	int col;
	for (col = 0; col < COLUMNS; col++) {
        	if (getrow(board, col) != -1) return 0;
    	}
    	return 1;
}

int evaluateWindow(int window[4], int player) {
	int other_player = (player == 1 ? 2 : 1), score = 0, playercnt = 0, emptycnt = 0, opponentcnt = 0, i;
	for (i = 0; i < 4; i++) {
		if (window[i] == player) playercnt++;
		else if (window[i] == 0) emptycnt++;
		else opponentcnt++;
	}
	if (playercnt == 4) score += WIN_SCORE;
	else if (playercnt == 3 && emptycnt == 1) score += 5;
	else if (playercnt == 2 && emptycnt == 2) score += 2;
	if (opponentcnt == 3 && emptycnt == 1) score -= 4;
	return score;
}

int evaluatePosition(int board[][COLUMNS], int player) {
	int score = 0, row, col, i;
    	int window[4];
    
    	// favor center column placements
    	for (row = 0; row < ROWS; row++) {
        	if (board[row][COLUMNS / 2] == player) score += 3;
    	}
    
    	// eval horizontal windows
    	for (row = 0; row < ROWS; row++) {
        	for (col = 0; col <= COLUMNS - 4; col++) {
            		for (i = 0; i < 4; i++) window[i] = board[row][col+i];
            		score += evaluateWindow(window, player);
        	}
    	}	
    
    	// eval vertical windows
    	for (row = 0; row <= ROWS - 4; row++) {
        	for (col = 0; col < COLUMNS; col++) {
            		for (i = 0; i < 4; i++) window[i] = board[row+i][col];
            		score += evaluateWindow(window, player);
        	}
    	}
    
    	// eval diagonal right windows
    	for (row = 0; row <= ROWS - 4; row++) {
        	for (col = 0; col <= COLUMNS - 4; col++) {
            		for (i = 0; i < 4; i++) window[i] = board[row+i][col+i];
            		score += evaluateWindow(window, player);
        	}
    	}
    
    	// eval diagonal left windows
    	for (row = 3; row < ROWS; row++) {
        	for (col = 0; col <= COLUMNS - 4; col++) {
            		for (i = 0; i < 4; i++) window[i] = board[row-i][col+i];
            		score += evaluateWindow(window, player);
        	}
    	}
    
    	return score;
}

// minimax algorithm with alpha-beta pruning
int minimax(int board[][COLUMNS], int depth, int alpha, int beta, int maximizingPlayer, int aiplayer) {
	if (checkWin(board, aiplayer)) return WIN_SCORE;
	int humanplayer = aiplayer == 1 ? 2 : 1;
    	if (checkWin(board, humanplayer)) return -WIN_SCORE;
    	if (isBoardFull(board) || depth == 0) return evaluatePosition(board, aiplayer);
	if (maximizingPlayer) {
		int maxEval = -INF, col;
		for (col = 0; col < COLUMNS; col++) {
			int row = getrow(board, col);
			if (row != -1) {
				board[row][col] = aiplayer;
				int eval = minimax(board, depth - 1, alpha, beta, 0, aiplayer);
				board[row][col] = 0;
				maxEval = (eval > maxEval) ? eval : maxEval;
				alpha = (alpha > maxEval) ? alpha : maxEval;
				if (beta <= alpha) break;
			}
		}
		return maxEval;
	} else {
		int minEval = INF, col;
		for (col = 0; col < COLUMNS; col++) {
			int row = getrow(board, col);
			if (row != -1) {
				board[row][col] = humanplayer;
				int eval = minimax(board, depth - 1, alpha, beta, 1, aiplayer);
				board[row][col] = 0;
				minEval = (eval < minEval) ? eval : minEval;
				beta = (beta < minEval) ? beta : minEval;
				if (beta <= alpha) break;
			}
		}
		return minEval;
	}

}

int findbest(int board[][COLUMNS], int player) {
	int bestScore = -INF, col, i;
	int bestCol = COLUMNS / 2;
	//lvl1
	for (col = 0; col < COLUMNS; col++) {
        	int row = getrow(board, col);
        	if (row != -1) {
            		board[row][col] = player;
            		if (checkWin(board, player)) {
                		board[row][col] = 0;
                		return col;
            		}
            		board[row][col] = 0;
        	}
    	}
	//lvl2
	int other_player = player == 1 ? 2 : 1;
	for (col = 0; col < COLUMNS; col++) {
        	int row = getrow(board, col);
        	if (row != -1) {
            		board[row][col] = other_player;
            		if (checkWin(board, other_player)) {
                		board[row][col] = 0;
                		return col;
            		}	
            		board[row][col] = 0;
        	}
    	}
	// when multiple moves equally good
	int moveOrder[COLUMNS];
    	for (i = 0; i < COLUMNS; i++) moveOrder[i] = i;

    	// shuffle
    	for (i = COLUMNS - 1; i > 0; i--) {
       		int j = rand() % (i + 1);
        	moveOrder[i] = moveOrder[i] ^ moveOrder[j];
       		moveOrder[j] = moveOrder[j] ^ moveOrder[i];
        	moveOrder[i] = moveOrder[i] ^ moveOrder[j];
    	}
	for (i = 0; i < COLUMNS; i++) {
        	int col = moveOrder[i];
        	int row = getrow(board, col);
        	if (row != -1) {
            		board[row][col] = player;
            		// evaluate with minimax
            		int score = minimax(board, MAX_DEPTH, -INF, INF, 0, player);
            		board[row][col] = 0;
            		// If we found a better move or if the score is equal but this column is closer to center
            		if (score > bestScore ||
                	(score == bestScore && abs(col - COLUMNS / 2) < abs(bestCol - COLUMNS / 2))) {
                		bestScore = score;
                		bestCol = col;
            		}
        	}
    	}
	return bestCol;

}

// This function chooses a move for the computer player,
// given the state of the board and the player number the
// computer is playing with.
// It supports two modes: 0 for making random valid moves
// and 1 for using a more advanced algorithm.
// It returns the move chosen (a value between 0 an COLUMNS-1).
int computer_move(int board[][COLUMNS], int player, int mode) {
	if (!mode) {
		int canmove = 0, i;
		for (i = 0; i < COLUMNS; i++) if (valid_move(board, i)) canmove = 1;
		if (canmove) {
			int n = rand() % 7;
			while (!valid_move(board, n)) n = rand() % 7;
			return n;
		} else return -1; // all columns full
	}
	else {
		struct timespec start, end;
		double time;
		clock_gettime(CLOCK_MONOTONIC, &start);
		int bestmove = findbest(board, player);
		clock_gettime(CLOCK_MONOTONIC, &end);
		time = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;
		printf("Move made in %.2f microseconds \n", time);
		return bestmove;
	}
}	


// Checks whether there is a winning player on the board.
// It gets passed the last move that was made.
// It returns 0 if there is no winner, or the otherwise the number
// of the player who won (1 or 2).
int check_winning(int board[][COLUMNS], int last_move) {
	int row;
	for (row = 0; row < ROWS; row++) {
		if (board[row][last_move]) break;
	}
	int player = board[row][last_move], d;
	int dirs[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
       	for (d = 0; d < 4; d++) {
		int dx = dirs[d][0], dy = dirs[d][1];
		int cnt = 1, i;
		for (i = 1; i < 4; i++) {
			int r = row + dx * i, c = last_move + dy * i;
			if (r < 0 || r >= ROWS || c < 0 || c >= COLUMNS || board[r][c] != player) break;
			cnt++;
		}
		for (i = 1; i < 4; i++) {
			int r = row - dx * i, c = last_move - dy * i;
			if (r < 0 || r >= ROWS || c < 0 || c >= COLUMNS || board[r][c] != player) break;
			cnt++;
		}
		if (cnt >= 4) return player;
	}
	return 0;
}




	
