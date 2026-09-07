#include "engine/CGameScriptThing.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharStringData {
    void* p0;
    int   v4;
};

extern CCharStringData* g_nullThing;
extern "C" int __fastcall Thing_Compare(void* a, void* b);

bool __fastcall CGameScriptThing_IsNull(const CGameScriptThing* self)
{
    CCharStringData* a = self->Name_PStringData;
    CCharStringData* b = g_nullThing;
    if (a == b)
        return true;
    if (a != 0 && b != 0 && a->v4 == b->v4)
        return (bool)(Thing_Compare(a->p0, b->p0) == 0);
    return false;
}