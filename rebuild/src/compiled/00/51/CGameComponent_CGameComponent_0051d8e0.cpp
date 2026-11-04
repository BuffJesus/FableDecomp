#include "fable_game.h"

// Retail 0x0051D8E0. RTTI and both callers identify CGameComponent;
// the propagated CAIStateGroup_GazeAtHome label at this address was wrong.
CGameComponent::CGameComponent(CGame& game)
    : Quit(false), Running(false), Game(&game)
{
}