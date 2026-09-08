#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CTrapInterfaceEntry { int Key; void* Interface; };
struct CTrapInterfaceMap {
    CTrapInterfaceEntry* Begin;
    CTrapInterfaceEntry* End;
    CTrapInterfaceEntry* LowerBound(const int* key);
};
struct CTrapTargetView {
    unsigned char _pad_0x00[0x2c];
    unsigned int TypeFlags;
    unsigned char _pad_0x30[0x14];
    CTrapInterfaceMap Interfaces;
};
struct CGameScriptInterfaceTrapVTable {
    unsigned char _pad_0x00[0x2c];
    CTrapTargetView* (__fastcall *GetTarget)(CGameScriptInterface*);
};
extern "C" void __fastcall Iface_Do(void* self);

bool __stdcall CGameScriptInterface_ManuallyTriggerTrap(CGameScriptInterface* self)
{
    CTrapTargetView* target =
        ((CGameScriptInterfaceTrapVTable*)self->__vftable)->GetTarget(self);
    if (target && (target->TypeFlags & 0x20000000)) {
        CTrapInterfaceMap* map = &target->Interfaces;
        int key = 0x7d;
        CTrapInterfaceEntry* entry = map->LowerBound(&key);
        CTrapInterfaceEntry* end = map->End;
        if (entry == end || entry->Key > 0x7d)
            entry = end;
        Iface_Do(entry->Interface);
        return true;
    }
    return false;
}
