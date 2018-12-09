#pragma once

#include "tetris.h"


//puts the color of a piece as the relevent elements of the piece, makes changing shapes easier
void setupPiece(Tetromino *piece)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			piece->shape[i][j] *= piece->color;
		}
	}
}

//puts a piece on the grid, or takes it out of the grid if erase_mode is true
void drawPiece(Tetromino *piece, bool erase_mode)
{
	short x1 = piece->pos_x, x2 = x1 + 4;
	short y1 = piece->pos_y, y2 = y1 + 4;

	if (erase_mode) {
		for (int y = y1; y < y2; y++) {
			for (int x = x1; x < x2; x++) {
				if(!((Game.grid[y][x] == Game.active_piece_ptr->shape[y-y1][x-x1] || Game.grid[y][x] == GAME_COLOR) && piece->shadow == 0)) {
				Game.grid[y][x] -= piece->shape[y - y1][x - x1];
			}
			}
		}
	} else {
		for (int y = y1; y < y2; y++) {
			for (int x = x1; x < x2; x++) {
				if (Game.grid[y][x] == (piece->shadow + GAME_COLOR) && piece->shadow > 0 && piece->shape[y-y1][x-x1] > 0) {
					Game.grid[y][x] -= piece->shadow;
				}
				Game.grid[y][x] += piece->shape[y - y1][x - x1];

			}
		}
	}
}

//tries lowers a piece one block, if it can't, it begins locking, if it can it draws the result
void pieceDrop(Tetromino *piece)
{

        //first we erase the piece where it was
	drawPiece(piece, true);
	piece->pos_y++;
        //then we place the piece where we want it to go
	drawPiece(piece, false);

	short x1 = piece->pos_x, x2 = x1 + 4;
	short y1 = piece->pos_y, y2 = y1 + 4;

        //collision detection loop sees if by lowering the piece we hit another piece
	for (int y = y1; y < y2; y++) {
		for (int x = x1; x < x2; x++) {
			if ((Game.grid[y][x]) > (piece->color + GAME_COLOR) && piece->shape[y - y1][x - x1] > 0 ) {

                                //if the piece caused a collision, we erase it, and move it back up
				drawPiece(piece, true);
				piece->pos_y--;
				drawPiece(piece, false);
                                //we change the flags to tell the main loop that we can't drop anymore
				Game.piece_falling = false;
				Game.piece_locking = true;
				return;

			}




		}
	}

	//if no collision occured we make sure the flags reflect that more drops are possible
	Game.piece_locking = false;
	Game.piece_falling = true;


}

//drops the piece as low as it can by watching the falling flag
void pieceHardDrop(Tetromino *piece)
{
	while (Game.piece_falling) {
		pieceDrop(piece);
	}
}


//
static void drawShadow(Tetromino *piece, bool erase) {
	Tetromino temp;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp.shape[j][i] = (piece->shape[j][i]);
		}
	}

	temp.pos_x = piece->pos_x;
	temp.pos_y = piece->pos_y;
	temp.color = piece->shadow;
	temp.shadow = 0;
	setupPiece(&temp);

	if(erase && Game.piece_falling) {
		temp.pos_y = piece->pos_s;
		drawPiece(&temp,true);
	} else {

	drawPiece(piece,true);
	drawPiece(&temp,false);
	pieceHardDrop(&temp);

	Game.piece_falling = true;
	Game.piece_locking = false;
	piece->pos_s = temp.pos_y;

	drawPiece(piece,false);
}
}

//prepares a piece to be dropped by drawing a new one at the top
void spawn(Tetromino *piece)
{

	s16 x1 = PIECE_START_X_POSITION;
    s16 x2 = x1 + 4;
	s16 y1 = PIECE_START_Y_POSITION;
    s16 y2 = y1 + 4;

	for (int y = y1; y < y2; y++) {
		for (int x = x1; x < x2; x++) {
			if (Game.grid[y][x] > GAME_COLOR ) {

                                //DEBUG
				//printf("game over at %d,%d\n", x, y );

                Game.piece_falling = false;
				Game.piece_locking = false;
				Game.active_piece_ptr = false;
				Game.game_over = true;

                                return;
			}
		}
	}

	piece->pos_x = PIECE_START_X_POSITION;
	piece->pos_y = PIECE_START_Y_POSITION;
	Game.active_piece_ptr = piece;
	Game.piece_falling = true;

	drawPiece(piece, false);
	drawShadow(piece, false);
}

//tries to move the piece right with a routine very similar to the drop routine above
bool moveRight(Tetromino *piece)
{
	if (Game.piece_falling) {
		drawShadow(piece, true);
	}
	drawPiece(piece, true);
	piece->pos_x++;
	short x1 = piece->pos_x, x2 = x1 + 4;
	short y1 = piece->pos_y, y2 = y1 + 4;
	drawPiece(piece, false);

	for (int y = y1; y < y2; y++) {
		for (int x = x1; x < x2; x++) {
			if (Game.grid[y][x] > (piece->color + GAME_COLOR) && piece->shape[y - y1][x - x1] > 0) {

				drawPiece(piece, true);
				piece->pos_x--;
				drawPiece(piece, false);

				if (Game.piece_falling) {
					drawShadow(piece, false);
				}

				return false;

			}
		}
	}

	if (Game.piece_falling) {
		drawShadow(piece,false);
	}

	return true;
}

//tries to move the piece left with a routine very similar to the drop routine above
bool moveLeft(Tetromino *piece)
{
	if(Game.piece_falling) {
		drawShadow(piece, true);
	}
	drawPiece(piece, true);
	piece->pos_x--;
	short x1 = piece->pos_x, x2 = x1 + 4;
	short y1 = piece->pos_y, y2 = y1 + 4;
	drawPiece(piece, false);

	for (int y = y1; y < y2; y++) {
		for (int x = x1; x < x2; x++) {
			if (Game.grid[y][x] > (piece->color + GAME_COLOR) && piece->shape[y - y1][x - x1] > 0) {

				drawPiece(piece, true);
				piece->pos_x++;
				drawPiece(piece, false);
				if (Game.piece_falling) {
					drawShadow(piece, false);
				}
				return false;

			}
		}
	}
		if (Game.piece_falling) {
			drawShadow(piece, false);
		}
	return true;
}

//makes sure the piece is actually at the bottom by trying to drop it first, then locks the piece
void pieceLock(Tetromino *piece)
{
        pieceDrop(piece);

        if (Game.piece_locking) {
		piece->pos_s = piece->pos_y;
                Game.active_piece_ID = 0;
        	Game.piece_locking = false;
        } else {
		drawShadow(piece,false);
	}

}

//tries to rotate the piece clockwise once using a temporary matrix so the change can be undone
void rotate(Tetromino *piece)
{
	//defining a temporary Tetromino so we can keep the previous state of the piece later on
	Tetromino temp;

	//the temp will inherit the values of the piece passed into the function
	temp.pos_x = piece->pos_x;
    temp.pos_y = piece->pos_y;
    temp.color = piece->color;
	temp.pos_s = piece->pos_s; 
    temp.shadow = piece->shadow;

	short x1 = piece->pos_x, x2 = x1 + 4;
	short y1 = piece->pos_y, y2 = y1 + 4;

	if (Game.piece_falling) {
		drawShadow(piece, true);
	}

	drawPiece(piece, true);

        //these loops set the shape of the temp Tetromino to the piece shape rotated clockwise
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp.shape[j][i] = piece->shape[3 - i][j];
		}
	}

		drawPiece(&temp, false);


        //check collision and undo if a collision occured
	for (int y = y1; y < y2; y++) {

		for (int x = x1; x < x2; x++) {

			if (Game.grid[y][x] > (temp.color + GAME_COLOR) && temp.shape[y - y1][x - x1] > 0) {

				drawPiece(&temp, true);
				drawPiece(piece, false);
				if (Game.piece_falling) {
					drawShadow(piece, false);
				}
				return;
			}
		}
	}

        //if no collision happened we make the rotated temp shape the new piece shape
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			piece->shape[i][j] = temp.shape[i][j];
		}
	}

	if (Game.piece_falling) {
		drawShadow(piece, false);
	}
}
