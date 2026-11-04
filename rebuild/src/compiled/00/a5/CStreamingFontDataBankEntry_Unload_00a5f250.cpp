// lib_streaming_font_data_bank.cpp, retail 0x00A5F150 / 0x00A5F250 (Ego_r
// ~CStreamingFontDataBankEntry and ?Unload@CStreamingFontDataBankEntry@@UAEXXZ):
// drop this entry's font reference. Same bodies as CFontDataBankEntry's
// (0x00A63480 / 0x00A63580); CCountedPointer<CFontBank>::ReleaseData is inlined.
#include "engine/CStreamingFontDataBankEntry.h"

template<> inline void CCountedPointer<CFontBank>::ReleaseData()
{
    Release();
    Info = 0;
    Data = 0;
}

void CStreamingFontDataBankEntry::Unload()
{
    Font.ReleaseData();
}

// Retail 00A5F150: Unload, then the Font member and base destructors.
CStreamingFontDataBankEntry::~CStreamingFontDataBankEntry()
{
    Unload();
}

// Retail 00A5F220 (vtable slot 1; the index lists the body under a stand-in
// name, 00/a5/CStreamingFontDataBankEntry_IsLoaded_00a5f220.cpp).
bool CStreamingFontDataBankEntry::IsLoaded() const
{
    return !Font.IsNull();
}
