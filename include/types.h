#pragma once

#include "tetris.h"

//
//  graphics related definiations
//

static const u32 GRID_X_POS = 100;
static const u32 GRID_Y_POS = 100;

typedef enum  TetrisColor
{
    GAME_COLOR = RGBA8_MAXALPHA(0, 0, 0),
    FRAME_COLOR = RGBA8_MAXALPHA(1, 1, 1),
    OUTLINE_COLOR_1 = 0b1011110111110111,
    OUTLINE_COLOR_2 = RGBA8_MAXALPHA(255, 255, 255),
    BACKGROUND_COLOR = 0x3ACE,
    TITLE_WHITE = RGBA8_MAXALPHA(255, 255, 255),
    TITLE_GRAY = 0x41E8,

    I_PIECE_COLOR = 0b0111100010000100,
    L_PIECE_COLOR = 0b0001111011100011,
    O_PIECE_COLOR = 0b0001100111110111,
    S_PIECE_COLOR = 0b0111100111100100,
    T_PIECE_COLOR = 0b0000011011110111,
    J_PIECE_COLOR = 0b0111100001110111,
    Z_PIECE_COLOR = 0b0111101011110111,

    I_SHADOW_COLOR = 0b0010000000000000,
    L_SHADOW_COLOR = 0b0000000100000000,
    O_SHADOW_COLOR = 0b0000000000000100,
    S_SHADOW_COLOR = 0b0010000010000000,
    T_SHADOW_COLOR = 0b0000000010000100,
    J_SHADOW_COLOR = 0b0010000000000100,
    Z_SHADOW_COLOR = 0b0010000010000100,

    COLOR_GRADIENT = RGBA8_MAXALPHA(1, 1, 1)
} TetrisColor;

typedef struct Point
{
    u32 x;
    u32 y;
} Point;

typedef struct Line
{
    Point p1;
    Point p2;
} Line;

typedef Line Rectangle;

typedef struct Screen
{
    u32 height;
    u32 width;
    u32 * buffer;
} Screen;

typedef enum NextState
{
    EXIT = -1,
    DO_PAUSE,
    DO_UPDATE,
    DO_TITLE
} NextState;


//
//  piece related definitions
//

static const u8 PIECE_START_X_POSITION = 4;
static const u8 PIECE_START_Y_POSITION = 0;

typedef struct Tetromino {
	u32 shape[4][4];
	s32 pos_x;
	s32 pos_y;
	TetrisColor color;
	TetrisColor shadow;
	s32 pos_s;
}Tetromino;

	//declaration of all tetrominos, they are static to ensure functions in the header can edit values
	static Tetromino I =
	{
		{ { 0, 0, 0, 0 },
		    { 1, 1, 1, 1 },
		    { 0, 0, 0, 0 },
		    { 0, 0, 0, 0 } },   //shape
		0,                      //pos_x
		0,                      //pos_y
		I_PIECE_COLOR,          //color
		I_SHADOW_COLOR,         //shadow
		0
	};

	static Tetromino L =
	{
		{ { 0, 0, 0, 0 },
		    { 0, 0, 1, 0 },
		    { 1, 1, 1, 0 },
		    { 0, 0, 0, 0 } },   //shape
		0,                      //pos_x
		0,                      //pos_y
		L_PIECE_COLOR,          //color
		L_SHADOW_COLOR,         //shadow
		0
	};

	static Tetromino O =
	{
		{ { 0, 0, 0, 0 },
		    { 0, 1, 1, 0 },
		    { 0, 1, 1, 0 },
		    { 0, 0, 0, 0 } },   //shape
		0,                      //pos_x
		0,                      //pos_y
		O_PIECE_COLOR,          //color
		O_SHADOW_COLOR,         //shadow
		0
	};

	static Tetromino S =
	{
		{ { 0, 0, 0, 0 },
		    { 0, 1, 1, 0 },
		    { 1, 1, 0, 0 },
		    { 0, 0, 0, 0 } },   //shape
		0,                      //pos_x
		0,                      //pos_y
		S_PIECE_COLOR,          //color
		S_SHADOW_COLOR,         //shadow
		0
	};

	static Tetromino T =
	{
		{ { 0, 0, 0, 0 },
		    { 1, 1, 1, 0 },
		    { 0, 1, 0, 0 },
		    { 0, 0, 0, 0 } },   //shape
		0,                      //pos_x
		0,                      //pos_y
		T_PIECE_COLOR,          //color
		T_SHADOW_COLOR,         //shadow
		0
	};

	static Tetromino J =
	{
		{ { 0, 0, 0, 0 },
		    { 0, 1, 0, 0 },
		    { 0, 1, 1, 1 },
		    { 0, 0, 0, 0 } },   //shape
		0,                      //pos_x
		0,                      //pos_y
		J_PIECE_COLOR,          //color
		J_SHADOW_COLOR,         //shadow
		0
	};

	static Tetromino Z =
	{
		{ { 0, 0, 0, 0 },
		    { 1, 1, 0, 0 },
		    { 0, 1, 1, 0 },
		    { 0, 0, 0, 0 } },   //shape
		0,                      //pos_x
		0,                      //pos_y
		Z_PIECE_COLOR,          //color
		Z_SHADOW_COLOR,         //shadow
		0
	};

    //
    // core game definitions
    //

static const int GRID_WIDTH = 11;
static const int GRID_HEIGHT = 24;

static struct Game
{
	s8 active_piece_ID;
	Tetromino* active_piece_ptr;
	bool piece_locking;
	bool piece_falling;
	bool game_over;
	u32 grid[25][12];
} Game;