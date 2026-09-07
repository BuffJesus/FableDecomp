#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct Sub; extern void __fastcall gtm_sub(Sub*);
struct CThingManager; extern CThingManager* g_thingmgr;
CThingManager* __fastcall GetThingManager(CCreatureActionBase* self)
{
    gtm_sub(reinterpret_cast<Sub*>(reinterpret_cast<char*>(self) + 8));
    return g_thingmgr;
}