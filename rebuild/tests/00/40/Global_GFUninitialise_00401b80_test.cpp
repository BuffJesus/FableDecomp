#include <stdio.h>

#include "fable_gfuninitialise.h"

CInitBaseClass g_FableInitBase_013B83D0;
CBankFileManager g_FableBankFileManager_013CA79C;
CThreadedFileManager g_FableThreadedFileManager_013BC870;

namespace
{
    fable_u32 g_order[9] = {};
    fable_u32 g_count = 0;
    CSystemManager g_systemManager;
    CEnginePrimitiveManager g_primitiveManager;

    void Record(fable_u32 event)
    {
        if (g_count < 9)
            g_order[g_count++] = event;
    }
}

void CInitBaseClass::Uninitialise()
{
    Record(1);
}

void FABLE_FASTCALL FableFileInstallerSingletonRelease()
{
    Record(2);
}

void CBankFileManager::Close()
{
    Record(3);
}

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    Record(4);
    return &g_systemManager;
}

void CSystemManager::Uninitialise()
{
    Record(5);
}

void CThreadedFileManager::Uninitialise()
{
    Record(6);
}

void FABLE_FASTCALL UninitializeStreamingFontTextureCache()
{
    Record(7);
}

CEnginePrimitiveManager* FABLE_FASTCALL
FableGetEnginePrimitiveManager_00404A80()
{
    Record(8);
    return &g_primitiveManager;
}

void CEnginePrimitiveManager::Uninitialise()
{
    Record(9);
}

int main()
{
    GFUninitialise();

    const fable_u32 expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    if (g_count != 9)
        return 1;
    for (fable_u32 index = 0; index < 9; ++index)
    {
        if (g_order[index] != expected[index])
            return 2;
    }

    puts("FABLETLC_GF_UNINITIALISE_BEHAVIOR PASS");
    return 0;
}
