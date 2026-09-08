#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

class CCharString {
public:
    CCharString(const char* text, int length);
    ~CCharString();
};

struct CScriptThing;

#pragma pointers_to_members(full_generality, single_inheritance)
struct CGameScriptInterfaceDispatch { void member(); };
typedef void (CGameScriptInterfaceDispatch::*DispatchPMF)(
    const CScriptThing*, const CCharString*, bool);

struct CGameScriptInterfaceVTableView {
    unsigned char _pad_0x000[0x938];
    DispatchPMF EntitySetSleepEnabled;
};

void __fastcall CGameScriptInterface_EntitySetSleepEnabled(
    const CGameScriptInterface* self, int /*edx*/,
    const CScriptThing* entity, bool enabled)
{
    CCharString stateName("SG_SLEEP", -1);
    CGameScriptInterfaceDispatch* callee =
        (CGameScriptInterfaceDispatch*)const_cast<CGameScriptInterface*>(self);
    const CGameScriptInterfaceVTableView* vtable =
        (const CGameScriptInterfaceVTableView*)self->__vftable;
    (callee->*(vtable->EntitySetSleepEnabled))(entity, &stateName, enabled);
}
