#include <stdio.h>

#include "fable_string.h"

namespace
{
    int g_unassignCalls = 0;
    CCharStringData* g_observedStorage = 0;
}

fable_i32 g_CCharStringInstanceCount_013BD800 = 0;

void CCharString::UnassignString()
{
    ++g_unassignCalls;
    g_observedStorage =
        *reinterpret_cast<CCharStringData**>(this);
    *reinterpret_cast<CCharStringData**>(this) = 0;
}

static int VerifyStringRelease()
{
    CCharStringData storage = {};
    fable_u32 objectStorage =
        reinterpret_cast<fable_u32>(&storage);
    CCharString* value =
        reinterpret_cast<CCharString*>(&objectStorage);

    g_CCharStringInstanceCount_013BD800 = 1;
    value->~CCharString();

    if (g_unassignCalls != 1)
        return 1;
    if (g_observedStorage != &storage)
        return 2;
    if (objectStorage != 0)
        return 3;
    if (g_CCharStringInstanceCount_013BD800 != 0)
        return 4;
    return 0;
}

int main()
{
    const int result = VerifyStringRelease();
    if (result != 0)
    {
        printf("FABLETLC_CHAR_STRING_DESTRUCTOR_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_CHAR_STRING_DESTRUCTOR_BEHAVIOR PASS\n");
    return 0;
}
