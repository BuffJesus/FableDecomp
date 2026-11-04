#include "fable_world.h"
#include "fable_game.h"
namespace {
inline long FABLE_FASTCALL GetNumFramesBetweenChangingIdleModes()
{
    return static_cast<long>(CMainGameComponent::GetConstantFPS() * 30.0f);
}
}
void CTCCreatureModeManager::RemoveMode(unsigned long mode)
{
    bool removedCurrent = false;
    if (!PModes.empty() && PModes.front()->GetType() == mode) {
        removedCurrent = true;
        PModes.front()->OnDeactivate();
    }
    PModes.remove_if(CModeBase::CIsType(mode));
    if (removedCurrent && !PModes.empty()) {
        PModes.front()->OnActivate();
        CTCLook* look;
        if (GetParentThing().GetTC(&look)) look->UpdateTurningAnims();
    }
    TimeForNextIdleMode = CWorld::GetFrame() + GetNumFramesBetweenChangingIdleModes();
}
