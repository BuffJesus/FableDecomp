// CGameScriptInterface::ManuallyResetTrap @ 008950e0
// Retail: ?ManuallyResetTrap@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z (virtual, const).
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
#include "engine/CScriptThing.h"

// CThing has no non-quarantined shared header yet; only the fields this body reads.
struct CTrapResetInterfaceEntry { int Key; void* Interface; };
struct CTrapResetInterfaceMap {
    CTrapResetInterfaceEntry* Begin;
    CTrapResetInterfaceEntry* End;
    CTrapResetInterfaceEntry* LowerBound(const int* key);
};
struct CTrapResetTargetView {
    unsigned char _pad_0x00[0x2c];
    unsigned int TypeFlags;
    unsigned char _pad_0x30[0x14];
    CTrapResetInterfaceMap Interfaces;
};
// CScriptThing vtable slot 0x2c resolves the scripted thing to its CThing.
struct CScriptThingTrapResetVTable {
    unsigned char _pad_0x00[0x2c];
    CTrapResetTargetView* (__fastcall *GetTarget)(const CScriptThing*);
};
extern "C" void __fastcall Iface_Do(void* self);

struct CGameScriptInterface_Methods : CGameScriptInterface {
    bool ManuallyResetTrap(const CScriptThing& thing) const;
};

// The retail body never reads its CGameScriptInterface `this`.
bool CGameScriptInterface_Methods::ManuallyResetTrap(const CScriptThing& thing) const
{
    CTrapResetTargetView* target =
        ((CScriptThingTrapResetVTable*)thing.__vftable)->GetTarget(&thing);
    if (target && (target->TypeFlags & 0x20000000)) {
        CTrapResetInterfaceMap* map = &target->Interfaces;
        int key = 0x7d;
        CTrapResetInterfaceEntry* entry = map->LowerBound(&key);
        CTrapResetInterfaceEntry* end = map->End;
        if (entry == end || entry->Key > 0x7d)
            entry = end;
        Iface_Do(entry->Interface);
        return true;
    }
    return false;
}
