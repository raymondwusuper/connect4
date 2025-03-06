#include "connect4.h"
#include <stdio.h>

// Display our gameboard as ASCII characters
void display_board_ASCII(int board[][COLUMNS]) {

    int i,j;
    char symbol;

    // Diplay the actual game board
    printf("\n");
    for (i=0;i<ROWS;i++) {

	printf("     ");
        for (j=0;j<COLUMNS;j++) {
	    if (board[i][j] == 1)
		symbol = 'X';
	    else if (board[i][j] == 2)
 		symbol = 'O';
	    else
		symbol = '.';
            printf(" %c ",symbol);
        }
        printf("\n");
    }
    // Display the column numbers
    printf("     ");
    for (i=0;i<COLUMNS;i++)
         printf("___");
    printf("\n");
    printf("     ");
    for (i=0;i<COLUMNS;i++)
         printf(" %d ",i+1);
    printf("\n\n");

}
	
