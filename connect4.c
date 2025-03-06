#include <stdio.h>
#include <stdlib.h>
#include "connect4.h"

// Start the simulation
int main() {	

	int board[ROWS][COLUMNS] = {0};

	// Seed the random number generator
	srand(time(NULL));
	
	int last_move = 0, playerfirst = 0, p, c;
	printf("go first? ");
	scanf("%d", &playerfirst);
	if (playerfirst) {
		p = 1;
		c = 2;
	} else {
		p = 2;
		c = 1;
	}

	// Inform the user
	while (1) {
		if (playerfirst) {
			int move1 = player_move(board);
			printf("%d\n", move1);
			add_move_board(board, move1, 1);
			display_board_ASCII(board);
			last_move = move1;
			if (check_winning(board, last_move)) break;
		}
		int move2 = computer_move(board, c, 1);
		printf("%d\n", move2);
		add_move_board(board, move2, c);
	       	display_board_ASCII(board);
		last_move = move2;
		if (check_winning(board, last_move)) break;
		if (!playerfirst) {
			int move1 = player_move(board);
			printf("%d\n", move1);
			add_move_board(board, move1, 2);
			display_board_ASCII(board);
			last_move = move1;
			if (check_winning(board, last_move)) break;
		}	

	}


	// Display the game board
	printf("player %d won!\n", check_winning(board, last_move));
	display_board_ASCII(board);

	printf("\n");
	
}

