// CFontDataBankEntry_Methods::CanUnload  @ retail 0x00a63560
// __fastcall bool CanUnload(CFontDataBankEntry* this)

#include "engine/CFontDataBankEntry.h"  // retyped onto the PDB layout; byte parity re-verified
enum EFontLoadState { FONT_LOADED = 1 };

struct CCPPointerInfo {
    EFontLoadState state; // [+0]
};

struct CFontDataBankEntry_Methods : CFontDataBankEntry {
    // [+8]
    bool CanUnload();
};

bool CFontDataBankEntry_Methods::CanUnload()
{
    if (this->Font_Data != 0 && this->Font_Info != 0)
    {
        int s = this->Font_Info->state;
        if (s == FONT_LOADED)
            return s == FONT_LOADED;
    }
    return false;
}