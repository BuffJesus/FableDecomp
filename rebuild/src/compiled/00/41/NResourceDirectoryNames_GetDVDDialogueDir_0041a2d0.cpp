// NResourceDirectoryNames::GetDVDDialogueDir, retail 0x0041A2D0
// (FableWin ?GetDVDDialogueDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ):
// "Data\lang\<language folder>\". FableWin's source order puts it right after
// GetSoundDir (0x0041A2B0) and before GetHDDialogueDir (0x0041A330, same body).
#include "fable_resource_directories.h"
#include "fable_game_text_bank.h"

CWideString FABLE_FASTCALL NResourceDirectoryNames::GetDVDDialogueDir()
{
    return L"Data\\lang\\" + NLocalisation::GetLanguageFolder().ToWideString() + L"\\";
}