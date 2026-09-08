#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CBedThingView {
    unsigned char _pad_0x00[0x24];
    unsigned int TypeFlags;
    unsigned char _pad_0x28[0x91 - 0x28];
    unsigned char StateFlags;
};

typedef CBedThingView* (__fastcall *GetBedThingFn)(CGameScriptInterface*);

bool __stdcall CGameScriptInterface_IsThingABed(CGameScriptInterface* self)
{
    GetBedThingFn* vtable = (GetBedThingFn*)self->__vftable;
    CBedThingView* thing = vtable[0x2c / 4](self);
    if (thing != 0 && !(thing->StateFlags & 1) && (thing->TypeFlags & 0x200000))
        return true;
    return false;
}
