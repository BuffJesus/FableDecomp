// CMainGameComponent::InitEventPackageSaving @ 0x004160CC: create the recording
// file and write a one-player header.
#pragma optimize("s", on)
#include "fable_game.h"
bool CMainGameComponent::InitEventPackageSaving(const wchar_t* fileName)
{
    {
        const CWideString name(fileName);
        static_cast<CAFile&>(EventPackageFile).Open(name, FILE_ACCESS_CREATE, 2);
    }
    EventPackageFileHeader.NoPlayers = 1;
    static_cast<CAFile&>(EventPackageFile).Write(&EventPackageFileHeader, sizeof(EventPackageFileHeader), 0);
    SavingEventPackages = true;
    return true;
}
