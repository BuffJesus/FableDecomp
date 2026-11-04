// directory_names.cpp: the NResourceDirectoryNames getters still missing under
// their Ego_r names (Ego_r's address order gives each one's place). Several were
// landed before as stand-ins (FableGet*Directory_*, S::M); these carry the real
// symbols. The language-folder pair builds "Data\lang\<folder>\".
#include "fable_resource_directories.h"
#include "fable_game_text_bank.h"

// Retail 0041A0A0.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetMiscDir()
{
    return L"Data\\Misc\\";
}

// Retail 0041A120.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetFontBankDir()
{
    return L"Data\\lang\\" + NLocalisation::GetLanguageFolder().ToWideString() + L"\\";
}

// Retail 0041A180.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetCacheDir()
{
    return L"Data\\Cache\\";
}

// Retail 0041A290.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetBoneConfigDir()
{
    return L"Data\\Bones\\";
}

// Retail 0041A2B0.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetSoundDir()
{
    return L"Data\\Sound\\";
}

// Retail 0041A330.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetHDDialogueDir()
{
    return L"Data\\lang\\" + NLocalisation::GetLanguageFolder().ToWideString() + L"\\";
}

// Retail 0041A390.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetDVDSoundDir()
{
    return L"Data\\Sound\\";
}

// Retail 0041A3B0.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetHDSoundDir()
{
    return L"Data\\Sound\\";
}

// Retail 0041A3D0.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetHDMusicDir()
{
    return L"Data\\Sound\\";
}

// Retail 0041A3F0.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetDVDMusicDir()
{
    return L"Data\\Sound\\";
}

// Retail 0041A410.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetStaticMapDir()
{
    return L"Data\\Levels\\";
}
