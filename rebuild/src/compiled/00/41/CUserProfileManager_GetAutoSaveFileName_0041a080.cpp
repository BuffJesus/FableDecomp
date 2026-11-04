// NResourceDirectoryNames::GetDefDir, retail 0x0041A080
// (FableWin ?GetDefDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ; it follows
// GetLevelDir 0x0041A060 in FableWin's order).
#include "fable_resource_directories.h"

CWideString FABLE_FASTCALL NResourceDirectoryNames::GetDefDir()
{
    return L"Data\\Defs\\";
}
