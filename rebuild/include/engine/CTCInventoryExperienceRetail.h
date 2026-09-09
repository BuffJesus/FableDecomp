#ifndef FABLE_ENGINE_CTCINVENTORYEXPERIENCERETAIL_H
#define FABLE_ENGINE_CTCINVENTORYEXPERIENCERETAIL_H

// Retail Fable.exe contracts the prefix inherited by CTCInventoryExperience.
// GetOriginalExperience @ 0x0056ea9a independently reads this four-long group
// at +0x1ac..+0x1b8 in the same order named by Ego_r.pdb at +0x1f0..+0x1fc.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
struct CTCInventoryExperienceRetail {
    unsigned char _prefix[0x1ac];
    long OrigGeneral;
    long OrigStrength;
    long OrigSkill;
    long OrigWill;
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperienceRetail, OrigGeneral) == 0x1ac);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperienceRetail, OrigStrength) == 0x1b0);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperienceRetail, OrigSkill) == 0x1b4);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperienceRetail, OrigWill) == 0x1b8);

#endif // FABLE_ENGINE_CTCINVENTORYEXPERIENCERETAIL_H
