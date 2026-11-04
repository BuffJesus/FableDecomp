// Retail 009915E0. The old CWorld::SetMiniMapEnabled label was contradicted
// by both native window callbacks, which pass PSoundManager as the receiver.
#include "fable_sound_enabled.h"

void CASoundManager::UnknownSetEnabled_009915e0(bool enabled)
{
    FableSoundEnabledState& state = *reinterpret_cast<FableSoundEnabledState*>(this);
    if (state.Enabled == enabled)
        return;
    state.Enabled = enabled;
    if (!enabled)
        UnknownSlot18(0);
}
