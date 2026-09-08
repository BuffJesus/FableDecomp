#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CKillInterfaceEntry { int Key; void* Value; };
struct CKillInterfaceMap { CKillInterfaceEntry* Begin; CKillInterfaceEntry* End; };
struct CKillTargetView {
    unsigned char _pad_0x00[0x20];
    unsigned int TypeFlags;
    unsigned char _pad_0x24[0x20];
    CKillInterfaceMap Interfaces;
};

struct CScriptThingArgument;
struct CScriptThingArgumentVTable {
    unsigned char _pad_0x00[0x2c];
    int (__fastcall* GetValue)(CScriptThingArgument* self);
};
struct CScriptThingArgument { CScriptThingArgumentVTable* __vftable; };

struct CKillInterface;
struct CKillInteger { int Value; };
struct CKillMapKey { const int* Value; };

extern void* __fastcall Sub1(void* playerManager);
extern CKillTargetView* __fastcall Sub2(void* value);
extern CKillInterfaceEntry* __fastcall VMap_LowerBound(
    CKillInterfaceMap* self, CKillMapKey key);
extern void __fastcall Iface_Do(CKillInterface* self, CKillInteger value);

void __fastcall CGameScriptInterface_SetThingAsKilled(
    const CGameScriptInterface* self, int edx_unused,
    CScriptThingArgument* thing)
{
    int key;
    (void)edx_unused;
    void* value = Sub1(((CGameScriptInterface*)self)->PlayerManager);
    CKillTargetView* target = Sub2(value);
    if (target != 0 && (target->TypeFlags & 0x100000)) {
        CKillInterfaceMap* map = &target->Interfaces;
        CKillMapKey mapKey;
        key = 0x14;
        mapKey.Value = &key;
        CKillInterfaceEntry* entry = VMap_LowerBound(map, mapKey);
        CKillInterfaceEntry* end = map->End;
        if (entry == end || entry->Key > 0x14)
            entry = end;
        CKillInterface* interfaceValue = (CKillInterface*)entry->Value;
        CKillInteger boxed;
        boxed.Value = thing->__vftable->GetValue(thing);
        Iface_Do(interfaceValue, boxed);
    }
}
