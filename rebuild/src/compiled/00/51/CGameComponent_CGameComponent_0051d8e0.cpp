#include "fable_game.h"

// Retail RTTI and the +8/+9/+C stores identify this shared base constructor.
// The old propagated CAIStateGroup_GazeAtHome manifest label is not its owner.
CGameComponent::CGameComponent(CGame& game)
    : Quit(false), Running(false), Game(&game)
{
}
