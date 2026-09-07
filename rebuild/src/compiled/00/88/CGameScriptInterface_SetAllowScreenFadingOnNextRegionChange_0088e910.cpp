#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld {
    unsigned char _pad[0xdb];
    unsigned char allowScreenFadingOnNextRegionChange; // +0xdb
};
void __fastcall CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange(
        CGameScriptInterface* self, int edx_dummy, char val) {
    self->World->allowScreenFadingOnNextRegionChange = (unsigned char)val;
}