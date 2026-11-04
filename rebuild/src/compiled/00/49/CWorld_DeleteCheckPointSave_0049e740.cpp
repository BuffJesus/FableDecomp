// CWorld::DeleteCheckPointSave, retail 0x0049E740
// (FableWin ?DeleteCheckPointSave@CWorld@@SIXXZ): remove the profile's checkpoint
// autosave and, when present, its ".hs" companion file.
#include "fable_world.h"
#include "fable_user_profile.h"
#include "fable_filesystem.h"
#include "fable_system_manager.h"

void CWorld::DeleteCheckPointSave()
{
    CWideString pathName = CUserProfileManager::Get().GetAutoSaveCheckPointPathName();
    if (CAFile::PathExists(pathName))
    {
        CDriveManager::Get().DeleteFileW(pathName);
        CWideString hashPathName = pathName + L".hs";
        if (CAFile::PathExists(hashPathName))
            CDriveManager::Get().DeleteFileW(hashPathName);
    }
}