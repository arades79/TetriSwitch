#pragma once

#include "tetris.h"

//Write a pixel to a defined x and y position to a defined color
static void drawPixel(Point* point, TetrisColor color, Screen* active_screen)
{
    u32 pos = (point->y * active_screen->width) + point->x;
	u32* pixel_buffer = (u32*)(active_screen->buffer + pos);
	*(pixel_buffer) = (u32)color;
}

int abs(int value)
{
    u32 temp = value >> 31;     // make a mask of the sign bit
    value ^= temp;                   // toggle the bits if value is negative
    value += temp & 1;               // add one if value was negative
    return value;
}

int sgn(int val) {
    return ((0 < val) - (val < (0)));
}

static void drawLine(Line* line, TetrisColor color, Screen* active_screen) {
        int x1 = line->p1.x;
        int x2 = line->p2.x;
        int y1 = line->p1.y;
        int y2 = line->p2.y;
        int dx=x2-x1; /* the horizontal distance of the line */
        int dy=y2-y1; /* the vertical distance of the line */
        int dxabs=abs(dx);
        int dyabs=abs(dy);
        int sdx=sgn(dx);
        int sdy=sgn(dy);
        int x=dyabs>>1;
        int y=dxabs>>1;
        Point p = {x1, y1};

        if (dxabs>=dyabs) /* the line is more horizontal than vertical */
        {
                for(int i=0; i<dxabs; i++)
                {
                        y+=dyabs;
                        if (y>=dxabs)
                        {
                                y-=dxabs;
                                p.y+=sdy;
                        }
                        p.x+=sdx;
                        drawPixel(&p, color, active_screen);
                }
        }
        else /* the line is more vertical than horizontal */
        {
                for(int i=0; i<dyabs; i++)
                {
                        x+=dxabs;
                        if (x>=dyabs)
                        {
                                x-=dyabs;
                                p.x+=sdx;
                        }
                        p.y+=sdy;
                        drawPixel(&p, color, active_screen);
                }
        }
}

//Creates a 10x10 block at the given coordinates
static void drawBlock(Point p1, u8 size, TetrisColor color, Screen* screen)
{
	Point p2 = {p1.x + size, p1.y + size};
    Rectangle rect = {p1, p2};
	u32 color_bias=0;
	for (rect.p1.y = p1.y; rect.p1.y < rect.p2.y; rect.p1.y++) {
		for (rect.p1.x = p1.x; rect.p1.x < rect.p2.x; rect.p1.x++) {
			drawPixel(&rect.p1, color + color_bias, screen);
		}
		if (color) {
			color_bias+=COLOR_GRADIENT;
		}
	}
}

void drawSmallBlock(Point corner, TetrisColor color, Screen* active_screen)
{

	short x2 = corner.x + 3, y2 = corner.y + 3;
	for (int j = corner.y; j < y2; j++) {
		for (int i = corner.x; i < x2; i++) {
			drawPixel(&(Point){i, j}, color, active_screen);
		}
	}
}

static void drawOutlineComponent(Rectangle rect, short stage, TetrisColor color, Screen* active_screen) {
    Point p_start = rect.p1;
    Point p_end = rect.p2;

    Line line = {p_start, p_end};
    line.p1.y -= stage;
    line.p2.y -= stage;
    drawLine(&line, color, active_screen);   //top line
    
    line.p1.x = p_end.x + stage;
    line.p1.y = p_start.y;
    line.p2.x = p_end.x + stage;
    line.p2.y = p_end.y;
    drawLine(&line, color, active_screen);   //right line
    
    line.p1.x = p_end.x;
    line.p1.y = p_end.y + stage;
    line.p2.x = p_start.x;
    line.p2.y = p_end.y + stage;
    drawLine(&line, color, active_screen);   //bottom line
    
    line.p1.x = p_start.x - stage;
    line.p1.y = p_end.y;
    line.p2.x = p_start.x - stage;
    line.p2.y = p_start.y;
    drawLine(&line, color, active_screen);   //left line
    
    line.p1.x = p_end.x;
    line.p1.y = p_start.y - stage;
    line.p2.x = p_end.x + stage;
    line.p2.y = p_start.y;
    drawLine(&line, color, active_screen);   //upper right diagonal line
    
    line.p1.x = p_end.x + stage;
    line.p1.y = p_end.y;
    line.p2.x = p_end.x;
    line.p2.y = p_end.y + stage;
    drawLine(&line, color, active_screen);   //lower right diagonal line
    
    line.p1.x = p_start.x;
    line.p1.y = p_end.y + stage;
    line.p2.x = p_start.x - stage;
    line.p2.y = p_end.y;
    drawLine(&line, color, active_screen);   //lower left diagonal line
    
    line.p1.x = p_start.x - stage;
    line.p1.y = p_start.y;
    line.p2.x = p_start.x;
    line.p2.y = p_start.y - stage;
    drawLine(&line, color, active_screen);   //upper left diagonal line
}

void drawOutline(Rectangle rect, Screen* active_screen)
{
	//inner line
	drawOutlineComponent(rect, 1, OUTLINE_COLOR_1, active_screen);
	drawOutlineComponent(rect, 2, OUTLINE_COLOR_2, active_screen);
	drawOutlineComponent(rect, 3, OUTLINE_COLOR_1, active_screen);

    Point p = rect.p1;
	drawPixel(&p, OUTLINE_COLOR_1, active_screen);
    
    p.x = rect.p2.x;
	drawPixel(&p, OUTLINE_COLOR_1, active_screen);
    
    p.y = rect.p2.y;
	drawPixel(&p, OUTLINE_COLOR_1, active_screen);

    p.x = rect.p1.x;
	drawPixel(&p, OUTLINE_COLOR_1, active_screen);
	
    p.x = rect.p1.x - 2;
    p.y = rect.p1.y - 2;
	drawPixel(&p, OUTLINE_COLOR_1, active_screen);
	
    p.x = rect.p2.x + 2;
	drawPixel(&p, OUTLINE_COLOR_1, active_screen);

    p.y = rect.p2.y + 2;
	drawPixel(&p, OUTLINE_COLOR_1, active_screen);

    p.x = rect.p1.x - 2;
	drawPixel(&p, OUTLINE_COLOR_1, active_screen);
}

void drawBox(Rectangle rect, TetrisColor color, Screen* screen)
{

	Point p;
	for (p.x = rect.p1.x; p.x < rect.p2.x; p.x++) {
		for (p.y = rect.p1.y; p.y < rect.p2.y; p.y++) {
			drawPixel(&p, color, screen);
		}
	}
}

//just fills screen with a color
void fillScreen(TetrisColor color, Screen* screen)
{
    Rectangle rect = {{0, 0}, {screen->width, screen->height}};
    drawBox(rect, color, screen);
}

//Create a 10x20 size grid on the screen made up by 10x10 blocks.
void drawGrid(Screen* active_screen, u8 block_size)
{
    Point p;
	for (int i = 1; i < 11; i++) {
		for (int j = 4; j < 24; j++) {
            p.x = GRID_X_POS + (i * block_size);
            p.y = (j * block_size) - GRID_Y_POS;
			drawBlock(p, block_size, (TetrisColor) Game.grid[j][i], active_screen);
		}
	}
}

void drawTitle(Screen* screen)
{
	for(int x=115; x<200; x++) {
		for(int y=47; y<75; y++) {
        drawPixel(&(Point){x, y},TITLE_WHITE, screen);
    }
	}
	for(int x=143; x<171; x++) {
		for(int y=75; y<103; y++) {
        drawPixel(&(Point){x, y},TITLE_WHITE, screen);
    }
	}
	drawLine(&(Line){(Point){113,46},(Point){200,46}},TITLE_GRAY, screen);
	drawLine(&(Line){(Point){200,46},(Point){200,74}},TITLE_GRAY, screen);
	drawLine(&(Line){(Point){201,75},(Point){171,75}},TITLE_GRAY, screen);
	drawLine(&(Line){(Point){171,74},(Point){171,102}},TITLE_GRAY, screen);
	drawLine(&(Line){(Point){172,103},(Point){142,103}},TITLE_GRAY, screen);
	drawLine(&(Line){(Point){142,103},(Point){142,76}},TITLE_GRAY, screen);
	drawLine(&(Line){(Point){143,75},(Point){114,75}},TITLE_GRAY, screen);
	drawLine(&(Line){(Point){114,75},(Point){114,47}},TITLE_GRAY, screen);

	drawSmallBlock((Point){116,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){120,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){124,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){120,55},TITLE_GRAY, screen);			//T
	drawSmallBlock((Point){120,59},TITLE_GRAY, screen);
	drawSmallBlock((Point){120,63},TITLE_GRAY, screen);
	drawSmallBlock((Point){120,67},TITLE_GRAY, screen);

	drawSmallBlock((Point){132,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){132,55},TITLE_GRAY, screen);
	drawSmallBlock((Point){132,59},TITLE_GRAY, screen);
	drawSmallBlock((Point){132,63},TITLE_GRAY, screen);
	drawSmallBlock((Point){132,67},TITLE_GRAY, screen);
	drawSmallBlock((Point){136,51},TITLE_GRAY, screen);		//E
	drawSmallBlock((Point){136,59},TITLE_GRAY, screen);
	drawSmallBlock((Point){136,67},TITLE_GRAY, screen);
	drawSmallBlock((Point){140,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){140,67},TITLE_GRAY, screen);

	drawSmallBlock((Point){148,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){152,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){152,55},TITLE_GRAY, screen);
	drawSmallBlock((Point){152,59},TITLE_GRAY, screen);			//T
	drawSmallBlock((Point){152,63},TITLE_GRAY, screen);
	drawSmallBlock((Point){152,67},TITLE_GRAY, screen);
	drawSmallBlock((Point){156,51},TITLE_GRAY, screen);

	drawSmallBlock((Point){164,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){164,55},TITLE_GRAY, screen);
	drawSmallBlock((Point){164,59},TITLE_GRAY, screen);
	drawSmallBlock((Point){164,63},TITLE_GRAY, screen);
	drawSmallBlock((Point){164,67},TITLE_GRAY, screen);
	drawSmallBlock((Point){168,51},TITLE_GRAY, screen);		//R
	drawSmallBlock((Point){168,59},TITLE_GRAY, screen);
	drawSmallBlock((Point){172,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){172,55},TITLE_GRAY, screen);
	drawSmallBlock((Point){172,63},TITLE_GRAY, screen);
	drawSmallBlock((Point){172,67},TITLE_GRAY, screen);

	drawSmallBlock((Point){180,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){180,55},TITLE_GRAY, screen);
	drawSmallBlock((Point){180,59},TITLE_GRAY, screen);
	drawSmallBlock((Point){180,63},TITLE_GRAY, screen);			//I
	drawSmallBlock((Point){180,67},TITLE_GRAY, screen);

	drawSmallBlock((Point){188,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){188,55},TITLE_GRAY, screen);
	drawSmallBlock((Point){188,59},TITLE_GRAY, screen);
	drawSmallBlock((Point){188,67},TITLE_GRAY, screen);
	drawSmallBlock((Point){192,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){192,59},TITLE_GRAY, screen);		//S
	drawSmallBlock((Point){192,67},TITLE_GRAY, screen);
	drawSmallBlock((Point){196,51},TITLE_GRAY, screen);
	drawSmallBlock((Point){196,59},TITLE_GRAY, screen);
	drawSmallBlock((Point){196,63},TITLE_GRAY, screen);
	drawSmallBlock((Point){196,67},TITLE_GRAY, screen);

}