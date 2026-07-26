#include <stdio.h>

#include "fable_persistence.h"

void* g_CBaseVTable_01231710[1] = {0};

int main()
{
    CBase object = {0};
    FableRestoreCBaseVTable_0099A300(&object);
    if (object.vtable != g_CBaseVTable_01231710)
    {
        printf("FABLETLC_CBASE_RESTORE_B_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    printf("FABLETLC_CBASE_RESTORE_B_BEHAVIOR PASS\n");
    return 0;
}
