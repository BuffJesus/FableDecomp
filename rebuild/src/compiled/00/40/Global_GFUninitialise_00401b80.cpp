#include "fable_gfuninitialise.h"

void FABLE_CDECL GFUninitialise()
{
    g_FableInitBase_013B83D0.Uninitialise();
    FableFileInstallerSingletonRelease();
    g_FableBankFileManager_013CA79C.Close();
    GFGetSystemManager()->Uninitialise();
    g_FableThreadedFileManager_013BC870.Uninitialise();
    UninitializeStreamingFontTextureCache();
    FableGetEnginePrimitiveManager_00404A80()->Uninitialise();
}
