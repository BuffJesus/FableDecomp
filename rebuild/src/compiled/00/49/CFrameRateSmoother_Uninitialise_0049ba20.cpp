#include "fable_game.h"

void CFrameRateSmoother::Uninitialise()
{
    if (IsValid())
        Times.clear();
    CInitBaseClass::Uninitialise();
}