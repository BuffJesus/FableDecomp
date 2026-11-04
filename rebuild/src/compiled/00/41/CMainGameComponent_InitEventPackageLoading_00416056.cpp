// CMainGameComponent::InitEventPackageLoading @ 0x00416056: open the replay file and read its header.
#pragma optimize("s", on)
#include "fable_game.h"
bool CMainGameComponent::InitEventPackageLoading(const wchar_t* fileName)
{
    {
        const CWideString name(fileName);
        static_cast<CAFile&>(EventPackageFile).Open(name, FILE_ACCESS_READ, 2);
    }
    static_cast<CAFile&>(EventPackageFile).Read(&EventPackageFileHeader, sizeof(EventPackageFileHeader), 0);
    LoadingEventPackages = true;
    return true;
}
