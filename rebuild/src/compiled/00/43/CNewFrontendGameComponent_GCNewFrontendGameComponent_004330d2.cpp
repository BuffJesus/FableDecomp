// CNewFrontendGameComponent::~CNewFrontendGameComponent @ 0x0042F50B
// Same size-optimised TU as the ctor 0x0042EA8F. Everything after the
// Shutdown guard is compiler-generated member destruction in reverse
// declaration order, then the inline ~CGameComponent.
#pragma optimize("s", on)

#include "fable_game.h"

CNewFrontendGameComponent::~CNewFrontendGameComponent()
{
    if (!Quit)
        Shutdown();
}
