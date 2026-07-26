#include <stdio.h>

#include "fable_profile.h"

static int VerifyRetailProfilingNoOp()
{
    fable_u32 objectStorage = 0x12345678UL;
    const CCharString& label =
        *reinterpret_cast<const CCharString*>(&objectStorage);

    NProfileTimer::StartProfile(label, 0);

    if (objectStorage != 0x12345678UL)
        return 1;
    return 0;
}

int main()
{
    const int result = VerifyRetailProfilingNoOp();
    if (result != 0)
    {
        printf("FABLETLC_PROFILE_START_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_PROFILE_START_BEHAVIOR PASS\n");
    return 0;
}
