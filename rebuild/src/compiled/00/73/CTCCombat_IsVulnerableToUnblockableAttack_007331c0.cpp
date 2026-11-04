#include "fable_thing_components.h"
#include "fable_game.h"
bool CTCCombat::IsVulnerableToUnblockableAttack() const
{
    if (BlockingNumFrames > CMainGameComponent::GetConstantFPS() * 3.0f)
        return true;
    return false;
}
