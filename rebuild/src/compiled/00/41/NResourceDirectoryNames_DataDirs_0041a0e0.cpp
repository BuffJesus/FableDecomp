// directory_names.cpp, retail 0x0041A0E0 / 0x0041A6A0 (Ego_r
// NResourceDirectoryNames::GetBrushDir / GetTattooDataDir): fixed data
// directories, one literal each.
#include "fable_resource_directories.h"

// Retail 0041A0E0.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetBrushDir()
{
    return L"Data\\Brushes\\";
}

// Retail 0041A6A0.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetTattooDataDir()
{
    return L"Data\\Tattoos\\";
}
