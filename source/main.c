

#include "tetris.h"


int main(int argc, char **argv)
{
    

    Screen active_screen;
    //u32  cnt=0;
    NextState what_next = DO_TITLE;

    gfxInitDefault();
    setupGrid();
    setupPiece(&I);
    setupPiece(&L);
    setupPiece(&O);
    setupPiece(&S);
    setupPiece(&T);
    setupPiece(&J);
    setupPiece(&Z);

    

    //Set current resolution automatically depending on current/changed OperationMode. Only use this when using gfxInitResolution*().
    //gfxConfigureAutoResolutionDefault(true);

    while(appletMainLoop())
    {
        // refresh framebuffer
        active_screen.buffer = (u32*) gfxGetFramebuffer(&active_screen.width, &active_screen.height);

        what_next = handleInputs();
        switch (what_next)
        {
            case EXIT:
                gfxExit();
                return 0;
            case DO_PAUSE:
                // handle pause
                break;
            case DO_UPDATE:
                // re-draw pieces, update framebuf   
                break;
            case DO_TITLE:
                drawTitle(&active_screen);
                // put user to the title screen
                break;          
        }
        gfxFlushBuffers();
        gfxSwapBuffers();
    }

    gfxExit();
    return 0;
}
