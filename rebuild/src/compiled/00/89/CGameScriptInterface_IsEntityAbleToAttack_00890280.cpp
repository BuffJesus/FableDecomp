// CGameScriptInterface::IsEntityAbleToAttack(const CScriptThing&) const @ 00890280
// (labels_rtti_port: ?IsEntityAbleToAttack@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z).
#include "engine/CScriptThing.h"
#include "fable_world.h"

// Retail 0085f3e0 (__fastcall, CThing in ecx); name not yet confirmed.
extern char __fastcall Entity_CanAttack(const CThing* thing);

typedef CThing* (__fastcall* GetScriptThingFn)(const CScriptThing*);

// The retail body does not access its CGameScriptInterface `this`; this ABI-normalized
// free function therefore exposes only the one stack argument used by the body.
bool __stdcall CGameScriptInterface_IsEntityAbleToAttack(const CScriptThing* scriptThing)
{
    CThing* entity = ((GetScriptThingFn)((void**)scriptThing->__vftable)[0x2c / 4])(scriptThing);
    if (IS_THING_ALIVE(entity))
        return Entity_CanAttack(entity);
    return false;
}
