#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

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
struct CGameScriptInterfaceTrapResetVTable {
    unsigned char _pad_0x00[0x2c];
    CTrapResetTargetView* (__fastcall *GetTarget)(CGameScriptInterface*);
};
extern "C" void __fastcall Iface_Do(void* self);

bool __stdcall CGameScriptInterface_ManuallyResetTrap(CGameScriptInterface* self)
{
    CTrapResetTargetView* target =
        ((CGameScriptInterfaceTrapResetVTable*)self->__vftable)->GetTarget(self);
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
