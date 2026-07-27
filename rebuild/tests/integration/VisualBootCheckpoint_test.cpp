#include "fable_visual_boot.h"

#include <stdio.h>

extern "C" __declspec(dllimport)
FableInstanceHandle FABLE_STDCALL GetModuleHandleA(const char* moduleName);

int main()
{
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    if (!FableIsRetailVisualAssetEmbedded())
        return 1;
#else
    if (FableIsRetailVisualAssetEmbedded())
        return 1;
#endif

    char verificationCommand[] = "--verify-visual-resource";
    const long result = FableRunVisualBootCheckpoint(
        GetModuleHandleA(0),
        verificationCommand,
        0);
    if (result != 0)
        return static_cast<int>(result);

    printf("FABLETLC_VISUAL_BOOT_BEHAVIOR PASS\n");
    return 0;
}
