

#include "tetris.h"


int main(int argc, char **argv)
{
    
    Screen active_screen;
    //u32  cnt=0;

    gfxInitDefault();
    setupGrid();
    setupPiece(&I);
    setupPiece(&L);
    setupPiece(&O);
    setupPiece(&S);
    setupPiece(&T);
    setupPiece(&J);
    setupPiece(&Z);

    Tetromino* pieceArray[7] = {&I, &L, &O, &S, &T, &J, &Z};

    Game.game_over = false;
    Game.piece_falling = false;
    Game.piece_locking = false;    

    //Set current resolution automatically depending on current/changed OperationMode. Only use this when using gfxInitResolution*().
    //gfxConfigureAutoResolutionDefault(true);

    while(appletMainLoop())
    {
        // refresh framebuffer
        active_screen.buffer = (u32*) gfxGetFramebuffer(&active_screen.width, &active_screen.height);
        		fillScreen(BACKGROUND_COLOR, &active_screen);


        if (Game.piece_falling || Game.piece_locking)
        {
            handleInputs();
            if (Game.piece_falling)
            {
                pieceDrop(Game.active_piece_ptr);
            }
            else
            {
                pieceLock(Game.active_piece_ptr);
            }

            drawBox((Rectangle){(Point){190, 14}, (Point){250, 215}}, GAME_COLOR, &active_screen);
		    drawBox((Rectangle){(Point){79, 14}, (Point){180, 215}}, GAME_COLOR, &active_screen);
		    drawOutline((Rectangle){(Point){79, 14}, (Point){179, 214}}, &active_screen);
		    drawOutline((Rectangle){(Point){190, 14}, (Point){249, 214}}, &active_screen);
		    drawLine(&(Line){(Point){210, 133}, (Point){224, 133}}, OUTLINE_COLOR_2, &active_screen);
		    drawLine(&(Line){(Point){214, 173}, (Point){220, 173}}, OUTLINE_COLOR_2, &active_screen);
            drawGrid(&active_screen, 10);
        }
        else if (Game.game_over)
        {
            setupGrid();
        }
        else
        {
            u64 random = randomGet64();
            spawn(pieceArray[random % 7]);
        }

        
        gfxFlushBuffers();
        gfxSwapBuffers();
    }

    gfxExit();
    return 0;
}
