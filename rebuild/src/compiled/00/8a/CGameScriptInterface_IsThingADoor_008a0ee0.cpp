#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CDoorThingView {
    unsigned char _pad_0x00[0x24];
    unsigned char TypeFlags;
    unsigned char _pad_0x25[0x91 - 0x25];
    unsigned char StateFlags;
};

struct CGameScriptInterfaceDoorVTable {
    unsigned char _pad_0x00[0x2c];
    CDoorThingView* (__fastcall *GetThing)(CGameScriptInterface* self);
};

bool __stdcall CGameScriptInterface_IsThingADoor(CGameScriptInterface* self)
{
    CGameScriptInterfaceDoorVTable* vtable =
        (CGameScriptInterfaceDoorVTable*)self->__vftable;
    CDoorThingView* thing = vtable->GetThing(self);
    if (thing && !(thing->StateFlags & 1) && (thing->TypeFlags & 0x40))
        return true;
    return false;
}
