// NResourceDirectoryNames::GetHDMiscDir, retail 0x0041A0C0
// (FableWin ?GetHDMiscDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ; the getters sit in
// FableWin's source order, GetLevelDir 0x0041A060 first, one per 0x20 bytes).
#include "fable_resource_directories.h"

CWideString FABLE_FASTCALL NResourceDirectoryNames::GetHDMiscDir()
{
    return L"Data\\Misc\\";
}