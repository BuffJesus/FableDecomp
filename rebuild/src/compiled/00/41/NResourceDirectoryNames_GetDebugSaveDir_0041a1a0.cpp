// directory_names.cpp, retail 0x0041A1A0 (Ego_r
// ?GetDebugSaveDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ): the profile
// manager's debug save directory.
#include "fable_resource_directories.h"
#include "fable_user_profile.h"

CWideString FABLE_FASTCALL NResourceDirectoryNames::GetDebugSaveDir()
{
    return CUserProfileManager::Get().GetDebugSaveDir();
}
