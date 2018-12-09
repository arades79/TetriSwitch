#pragma once

#include "tetris.h"

static const int GRID_WIDTH = 11;
static const int GRID_HEIGHT = 24;

static struct Game {
	s8 active_piece_ID;
	Tetromino* active_piece_ptr;
	bool piece_locking;
	bool piece_falling;
	bool game_over;
	u32 grid[25][12];
} Game;

//puts the color of a piece as the relevent elements of the piece, makes changing shapes easier
void setupPiece(Tetromino *piece)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			piece->shape[i][j] *= piece->color;
		}
	}
}

// sets grid to be a defined color as well as have a border for collision detection purposes
void setupGrid()
{
	for (int i = 0; i < GRID_HEIGHT + 1; i++) {
		for (int j = 0; j < GRID_WIDTH + 1; j++) {
			Game.grid[i][j] = GAME_COLOR + FRAME_COLOR;
		}
	}
	for (int i = 0; i < GRID_HEIGHT; i++) {
		for (int j = 1; j < GRID_WIDTH; j++) {
			Game.grid[i][j] = GAME_COLOR;
		}
	}
}

void prepareGrid(Tetromino *piece, u8 block_size, Screen* active_screen)
{
    Point block;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
            block.x = GRID_X_POS + (i * block_size);
            block.y = (GRID_Y_POS + (j * block_size));
			drawBlock(block, block_size, (TetrisColor) piece->shape[j][i], active_screen);
		}
	}
}

//clears a row that has been completed, and drops all other rows above it
static void rowClear(u8 row)
{
	for (int i = 1; i < 11; i++) {
		Game.grid[row][i] = 0;
	}
	for (int j = row; j > 0; j--) {
		for (int i = 1; i < 11; i++) {
			Game.grid[j][i] = Game.grid[j - 1][i];
		}
	}
}


//checks for any completed rows and clear them
u8 checkRows()
{
	bool row_full;
	u8 row_clears = 0;

	for (int j = GRID_HEIGHT - 1; j > 1; j--) {
		row_full = true;
		for (int i = 0; i < 12; i++) {
			// if (Game.grid[j][i] == 0) {

            //                     //DEBUG
			// 	//printf("row %d not full\n", j );

			// 	row_full = false;
			// 	break;
			// }

            // possibly more efficient that above if, needs testing
            row_full = row_full && Game.grid[j][i]; 
		}

                //DEBUG
		//printf("row_product of row %d is %d\n", j, row_product );

		if (row_full) {
                        //DEBUG
			//printf("attempting to clear row %d\n", j );

			rowClear(j);
			row_clears++;
			j = GRID_HEIGHT; // restart loop to see if more rows need clearing
		}
	}

	return row_clears;
}

void itoa(unsigned int input, char string[])
{
	int i = 0;

	do {
		string[i] = (char)(input % 10) + '0';
		input /= 10;
		i++;
	} while (input);
	string[i] = '\0';

	//DEBUG
	//printf("%s\n", string);

	char c;

	i--;

	for (int j = 0; j <= i; j++, i--) {
		c = string[j];
		string[j] = string[i];
		string[i] = c;

	}

}

int sgn(int val) {
    return ((0 < val) - (val < (0)));
}

//very quick and efficient PRNG algorithm
int quickrand(int x)
{
	return x + ((x * x) | 5);
}

int abs(int value)
{
    u32 temp = value >> 31;     // make a mask of the sign bit
    value ^= temp;                   // toggle the bits if value is negative
    value += temp & 1;               // add one if value was negative
    return value;
}