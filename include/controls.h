#pragma once

#include "tetris.h"

typedef enum
{
    EXIT = -1,
    DO_PAUSE,
    DO_UPDATE,
    DO_TITLE
} NextState;

NextState handleInputs(void)
{
    //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        kDown |= hidKeysHeld(CONTROLLER_P1_AUTO);

        while (kDown)
        {
            switch(kDown)
            {
                case KEY_PLUS:
                case KEY_DLEFT:
                    // move piece left
                case KEY_DDOWN:
                    // fast drop 
            }
        }

        if (kDown & KEY_PLUS) return EXIT; // break in order to return to hbmenu
        else return DO_TITLE;
}