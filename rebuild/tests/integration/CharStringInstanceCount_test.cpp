#include "fable_string.h"
#include <stdio.h>

int main()
{
    if (g_CCharStringInstanceCount_013BD800 != 0) return 1;
    ++g_CCharStringInstanceCount_013BD800;
    if (g_CCharStringInstanceCount_013BD800 != 1) return 2;
    g_CCharStringInstanceCount_013BD800 = -3;
    if (g_CCharStringInstanceCount_013BD800 != -3) return 3;
    g_CCharStringInstanceCount_013BD800 = 0;
    puts("CHARSTRING_INSTANCE_COUNT_OK");
    return 0;
}
