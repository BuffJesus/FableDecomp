#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CRumbleTarget;
struct CRumbleTargetVTable {
    void* Slots[0x12];
    void (__fastcall *ClearRumbles)(CRumbleTarget*);
};
struct CRumbleTarget {
    CRumbleTargetVTable* __vftable;
    unsigned char _pad_0x04[0x91 - 4];
    unsigned char StateFlags;
};

struct CRumbleTableEntry {
    CRumbleTarget* Target;
    void* Other;
};
struct CRumbleManager {
    unsigned char _pad_0x00[0x2c];
    CRumbleTableEntry* Table;
};

extern CRumbleManager* g_manager;
extern void* __fastcall GetA(void* playerManager);
extern CRumbleTarget* __fastcall GetObj(void* value);
extern int __fastcall GetIndex(CRumbleTarget* target);

void __fastcall CGameScriptInterface_ClearAllRumbles(CGameScriptInterface* self)
{
    void* value = GetA(self->PlayerManager);
    CRumbleTarget* target = GetObj(value);
    if (target == 0)
        return;
    if (target->StateFlags & 1)
        return;
    int index = GetIndex(target);
    CRumbleTarget* entryTarget = g_manager->Table[index].Target;
    entryTarget->__vftable->ClearRumbles(entryTarget);
}
