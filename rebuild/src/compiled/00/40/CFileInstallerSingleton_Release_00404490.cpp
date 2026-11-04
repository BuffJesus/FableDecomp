#include "fable_file_installer.h"
#include "fable_boot.h"

void FABLE_FASTCALL CTBaseSingleton<CFileInstaller>::Release()
{
    if(g_CFileInstallerSingleton_013CA818.reference) {
        --static_cast<FableReferenceCount*>(g_CFileInstallerSingleton_013CA818.reference)->owners;
        if(static_cast<FableReferenceCount*>(g_CFileInstallerSingleton_013CA818.reference)->owners==0) {
            FableReferenceCount* info=static_cast<FableReferenceCount*>(g_CFileInstallerSingleton_013CA818.reference);
            info->destroy(info->object);
            FableOperatorDelete(g_CFileInstallerSingleton_013CA818.reference);
        }
    }
    g_CFileInstallerSingleton_013CA818.reference=0;
    g_CFileInstallerSingleton_013CA818.object=0;
}
