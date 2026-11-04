#include <stdio.h>
#include "fable_gfuninitialise.h"

// The singleton is raw storage here: its constructor is not under test.
#pragma comment(linker, "/alternatename:?GSystemManager@@3VCSystemManager@@A=_g_testSystemManagerStorage")
extern "C" double g_testSystemManagerStorage[sizeof(CSystemManager) / sizeof(double) + 1];
extern "C" double g_testSystemManagerStorage[sizeof(CSystemManager) / sizeof(double) + 1] = {};

int main()
{
    CSystemManager* first = GFGetSystemManager();
    CSystemManager* second = GFGetSystemManager();
    if (first != &GSystemManager || second != first ||
        static_cast<void*>(first) != static_cast<void*>(g_testSystemManagerStorage))
    {
        printf("FABLETLC_GET_SYSTEM_MANAGER_BEHAVIOR FAIL\n");
        return 1;
    }
    printf("FABLETLC_GET_SYSTEM_MANAGER_BEHAVIOR PASS\n");
    return 0;
}
