#include <stdio.h>

#include "fable_filesystem.h"

fable_i32 g_CStringInstanceCount_013BCA20 = 0;

CWideString::~CWideString()
{
}

static int VerifyDefaultStringConstruction()
{
    g_CStringInstanceCount_013BCA20 = 41;

    CWideString value;
    const fable_u32 storage =
        *reinterpret_cast<const fable_u32*>(&value);

    if (storage != 0)
        return 1;
    if (g_CStringInstanceCount_013BCA20 != 42)
        return 2;
    return 0;
}

int main()
{
    const int result = VerifyDefaultStringConstruction();
    if (result != 0)
    {
        printf("FABLETLC_WIDE_STRING_CONSTRUCTOR_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_WIDE_STRING_CONSTRUCTOR_BEHAVIOR PASS\n");
    return 0;
}
