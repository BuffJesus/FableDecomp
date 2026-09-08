#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CDeedReactionName {
    int Value;
    CDeedReactionName(int value, int length);
    ~CDeedReactionName();
};

typedef void (__fastcall *SetDeedReactionsFn)(
    CGameScriptInterface*, CDeedReactionName*, int,
    CDeedReactionName*, int);

struct CGameScriptInterfaceDeedVTable {
    SetDeedReactionsFn Slots[0x300];
};

void __fastcall CGameScriptInterface_EntitySetDeedReactionsEnabled(
    CGameScriptInterface* self, int, int entity, int enabled)
{
    CDeedReactionName name(0x1275988, -1);
    CGameScriptInterfaceDeedVTable* vtable =
        (CGameScriptInterfaceDeedVTable*)self->__vftable;
    vtable->Slots[0x938 / 4](self, &name, entity, &name, enabled);
}
