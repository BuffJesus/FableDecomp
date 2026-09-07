// CStreamingFontDataBankEntry_Methods::CanUnload  @ retail 0x00a63560
// __fastcall bool CanUnload(CStreamingFontDataBankEntry* this)

#include "engine/CStreamingFontDataBankEntry.h"  // retyped onto the PDB layout; byte parity re-verified
enum EFontLoadState { FONT_LOADED = 1 };

struct CCPPointerInfo {
    EFontLoadState state; // [+0]
};

struct CStreamingFontDataBankEntry_Methods : CStreamingFontDataBankEntry {
    // [+8]
    bool CanUnload();
};

bool CStreamingFontDataBankEntry_Methods::CanUnload()
{
    if (this->Font_Data != 0 && this->Font_Info != 0)
    {
        int s = this->Font_Info->state;
        if (s == FONT_LOADED)
            return s == FONT_LOADED;
    }
    return false;
}