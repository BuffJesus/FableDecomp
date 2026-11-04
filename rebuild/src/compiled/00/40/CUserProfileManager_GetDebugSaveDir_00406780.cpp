// user_profile_manager.cpp, retail 0x00406780 (Ego_r
// ?GetDebugSaveDir@CUserProfileManager@@QBE?AVCWideString@@XZ): "Data\" in
// front of the save directory.
#include "fable_user_profile.h"
#include "fable_resource_directories.h"

CWideString CUserProfileManager::GetDebugSaveDir() const
{
    return L"Data\\" + NResourceDirectoryNames::GetSaveDir();
}
