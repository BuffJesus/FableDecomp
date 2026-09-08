#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CSmashableThingView {
    unsigned char _pad_0x00[0x28];
    unsigned int TypeFlags;
    unsigned char _pad_0x2c[0x65];
    unsigned char StateFlags;
};

struct CGameScriptInterfaceSmashableVTable {
    void* Slots[11];
    CSmashableThingView* (__fastcall* GetThing)(void* self);
};

bool __stdcall CGameScriptInterface_IsThingSmashable(CGameScriptInterface* self)
{
    CGameScriptInterfaceSmashableVTable* vtable =
        (CGameScriptInterfaceSmashableVTable*)self->__vftable;
    CSmashableThingView* thing = vtable->GetThing(self);
    if (thing && !(thing->StateFlags & 1) && (thing->TypeFlags & 0x4000))
        return true;
    return false;
}
