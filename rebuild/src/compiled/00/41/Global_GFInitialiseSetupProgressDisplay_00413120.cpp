#include "fable_boot.h"

fable_u8 FABLE_FASTCALL GFInitialise_SetupProgressDisplay(
    GFInitialiseState* state)
{
    if (GFInitialiseState_Begin(state) != 0)
    {
        state->startupFlag20C = 0;
        state->startupValue08 = 0;

        CCountedProgressDisplay countedDisplay(new CProgressDisplay);
        SetProgressDisplay(&countedDisplay);
        return 1;
    }

    return 0;
}
