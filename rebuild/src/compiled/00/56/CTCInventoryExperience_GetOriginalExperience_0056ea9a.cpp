#pragma optimize("s",on)
#include "engine/CTCInventoryExperience.h"
#include "engine/CTCInventoryExperienceRetail.h"

long CTCInventoryExperience::GetOriginalExperience(long experienceType)
{
    const CTCInventoryExperienceRetail* retail =
        reinterpret_cast<const CTCInventoryExperienceRetail*>(this);

    if (experienceType == 0)
        return retail->OrigGeneral;
    if (experienceType == 1)
        return retail->OrigStrength;
    if (experienceType == 2)
        return retail->OrigSkill;
    if (experienceType == 3)
        return retail->OrigWill;
    return 0;
}