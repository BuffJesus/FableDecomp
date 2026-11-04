// lib_font_data_bank.cpp, retail 0x00A63480 / 0x00A63580 (Ego_r ~CFontDataBankEntry and
// ?Unload@CFontDataBankEntry@@UAEXXZ): drop this entry's font reference.
// CCountedPointer<CFontBank>::ReleaseData is inlined here, as retail does.
#include "engine/CFontDataBankEntry.h"

template<> inline void CCountedPointer<CFontBank>::ReleaseData()
{
    Release();
    Info = 0;
    Data = 0;
}

void CFontDataBankEntry::Unload()
{
    Font.ReleaseData();
}

// Retail 00A63480: Unload, then the Font member and base destructors.
CFontDataBankEntry::~CFontDataBankEntry()
{
    Unload();
}

// Retail 00A63550 (vtable slot 1; the index lists the folded body under a
// stand-in name).
bool CFontDataBankEntry::IsLoaded() const
{
    return !Font.IsNull();
}
