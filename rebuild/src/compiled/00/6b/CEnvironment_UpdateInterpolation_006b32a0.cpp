// CEnvironment::UpdateInterpolation (retail 0x006B32A0, Ego_r 0x006EEF40): CWorld::UpdateFrame's per-frame step for the
// environment's spell effects - forward to the spell-effect interface (FrameUpdate 0x00885010).
#include "fable_environment.h"

void CEnvironment::UpdateInterpolation()
{
    SpellEffectInterface->FrameUpdate();
}
