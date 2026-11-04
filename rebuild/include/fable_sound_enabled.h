#pragma once
#include "fable_sound_manager.h"

// Partial retail storage view, not an owning class definition. 009915E0 reads
// and writes +0x25; CSoundManagerLH::slot17 (00A3E4B0) reads the same flag.
// The original field name is not recovered. Keep the base-class size unchanged.
struct FableSoundEnabledState
{
    void* VTable;
    fable_u8 Unknown04[0x25 - 4];
    bool Enabled;
};
FABLE_STATIC_ASSERT(offsetof(FableSoundEnabledState, Enabled) == 0x25);
