#include <stdio.h>
#include <string.h>
#include "engine/CTCInventoryExperience.h"
#include "engine/CTCInventoryExperienceRetail.h"

int main()
{
    unsigned char storage[sizeof(CTCInventoryExperience)];
    CTCInventoryExperience* inventory =
        reinterpret_cast<CTCInventoryExperience*>(storage);
    CTCInventoryExperienceRetail* retail =
        reinterpret_cast<CTCInventoryExperienceRetail*>(storage);

    memset(storage, 0, sizeof(storage));
    retail->OrigGeneral = 101;
    retail->OrigStrength = 202;
    retail->OrigSkill = 303;
    retail->OrigWill = 404;

    if (inventory->GetOriginalExperience(0) != 101) return 1;
    if (inventory->GetOriginalExperience(1) != 202) return 2;
    if (inventory->GetOriginalExperience(2) != 303) return 3;
    if (inventory->GetOriginalExperience(3) != 404) return 4;
    if (inventory->GetOriginalExperience(-1) != 0) return 5;
    if (inventory->GetOriginalExperience(4) != 0) return 6;

    printf("ORIGINAL_EXPERIENCE PASS\n");
    return 0;
}