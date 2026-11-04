#include "fable_boot.h"
#include "fable_cgame.h"

fable_u8 FABLE_FASTCALL FableRunGFInitialiseProgressPhase(
    GFInitialiseProgressPhaseState& state)
{
    if (state.game->Initialise() != true)
        return 0;

    state.halfDisplayWidth = state.displayWidth / 2;
    return 1;
}
