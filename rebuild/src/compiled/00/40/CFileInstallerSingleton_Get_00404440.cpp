#include "fable_file_installer.h"

CFileInstaller& FABLE_FASTCALL CFileInstallerSingleton::Get()
{
    CFileInstaller* installer = g_CFileInstallerSingleton_013CA818.object;
    if (installer == 0)
    {
        g_CFileInstallerSingleton_013CA818 = new CFileInstaller;
    }

    return *g_CFileInstallerSingleton_013CA818.object;
}
