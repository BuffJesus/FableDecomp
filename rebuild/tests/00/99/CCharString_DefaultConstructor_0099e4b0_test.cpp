#include <stdio.h>

#include "fable_string.h"

fable_i32 g_CCharStringInstanceCount_013BD800 = 0;

CCharString::~CCharString()
{
}

static int VerifyDefaultStringConstruction()
{
    g_CCharStringInstanceCount_013BD800 = 8;

    CCharString value;
    const fable_u32 storage =
        *reinterpret_cast<const fable_u32*>(&value);

    if (storage != 0)
        return 1;
    if (g_CCharStringInstanceCount_013BD800 != 9)
        return 2;
    return 0;
}

int main()
{
    const int result = VerifyDefaultStringConstruction();
    if (result != 0)
    {
        printf("FABLETLC_CHAR_STRING_DEFAULT_CONSTRUCTOR_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_CHAR_STRING_DEFAULT_CONSTRUCTOR_BEHAVIOR PASS\n");
    return 0;
}
