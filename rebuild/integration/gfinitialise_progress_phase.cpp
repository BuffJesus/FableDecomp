#include "fable_boot.h"

fable_u8 FABLE_FASTCALL FableRunGFInitialiseProgressPhase(
    GFInitialiseProgressPhaseState& state)
{
    if (GFInitialise_SetupProgressDisplay(&state.progressDisplay) != 1)
        return 0;

    state.halfDisplayWidth = state.displayWidth / 2;
    return 1;
}
