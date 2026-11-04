#include "fable_resource_directories.h"

// directory_names.cpp, retail 0x0041A4C0-0x0041A69F: the "My Games\Fable\" user
// directories (GetUserDataDir 0041A4C0, GetSaveDirWithoutTrailingBlackslash
// 0041A4F0, GetSaveDir 0041A540, then the three below). Retail inlines the
// helpers here: each temporary dies before the next append.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetUserDataDir()
{
    return GetMyDocumentsDir() + L"My Games\\Fable\\";
}

CWideString FABLE_FASTCALL
NResourceDirectoryNames::GetSaveDirWithoutTrailingBlackslash()
{
    return GetUserDataDir() + L"Saves";
}

CWideString FABLE_FASTCALL NResourceDirectoryNames::GetSaveDir()
{
    return GetSaveDirWithoutTrailingBlackslash() + L"\\";
}

// Retail 0041A5B0.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetScoreboardImageCacheDir()
{
    return GetUserDataDir() + L"Scoreboard\\";
}

// Retail 0041A600.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetPhotojournalDir()
{
    return GetUserDataDir() + L"Photojournal\\";
}

// Retail 0041A650.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetTattooUserDir()
{
    return GetUserDataDir() + L"Tattoos\\";
}
