#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CSearchableThingView {
    unsigned char _pad_0x00[0x24];
    unsigned int TypeFlags;
    unsigned char _pad_0x28[0x91 - 0x28];
    unsigned char StateFlags;
};

typedef CSearchableThingView* (__fastcall *GetSearchableThingFn)(
    const CGameScriptInterface*);

bool __stdcall CGameScriptInterface_IsThingSearchable(
    const CGameScriptInterface* self)
{
    void** vtable = (void**)self->__vftable;
    CSearchableThingView* thing =
        ((GetSearchableThingFn)vtable[0x2c / 4])(self);
    if (thing != 0 && (thing->StateFlags & 1) == 0 &&
        (thing->TypeFlags & 0x10000000) != 0)
        return true;
    return false;
}
