#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld { int key; void* obj; };


struct Ent {
    char pad[0x28];
    int field28;      // +0x28
    char pad2[0x18];
    CGameScriptInterface vec;       // +0x44
};

struct IGSI {
    Ent* (**vt)();
};

struct KeyPtr { int* p; };

extern CWorld* __fastcall EntVec_LowerBound(CGameScriptInterface* self, KeyPtr key);
extern void __fastcall Obj_Release(void* self);

void __stdcall CGameScriptInterface_EntityClearPersonalityOverride(IGSI* self)
{
    Ent* e = ((Ent* (__fastcall*)(IGSI*))self->vt[0x2c/4])(self);
    if (!e) return;
    if (!(e->field28 & 0x8000)) return;
    int local = 0x4f;
    KeyPtr k; k.p = &local;
    CGameScriptInterface* v = &e->vec;
    CWorld* r = EntVec_LowerBound(v, k);
    CWorld* end = v->World;
    if (r != end && r->key <= 0x4f) {
        // keep r
    } else {
        r = end;
    }
    Obj_Release(r->obj);
}