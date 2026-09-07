// CGameScriptInterface::StartSneaking @ 008a1300
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Node { int key; void* p4; };
struct Container {
    Node* begin; Node* end_;
    Node* LowerBound(int* key); // __fastcall: ecx=this, key on stack
};

struct Obj {
    unsigned char _pad0[0x24];
    unsigned int field24;      // +0x24
    unsigned char _pad28[0x44 - 0x28];
    Container cont;            // +0x44 (begin), +0x48 (end)
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char flags;       // +0x91
};


struct M14 { char d; };

extern Obj* __fastcall Resolve1(M14* self);          // ecx = member14
extern Obj* __fastcall Resolve2(Obj* p);             // ecx = eax
extern void __fastcall FinalCall(void* p);           // ecx

void __fastcall CGameScriptInterface_StartSneaking(CGameScriptInterface* self)
{
    Obj* a = Resolve1((M14*)self->PlayerManager);
    Obj* o = Resolve2(a);
    if (!o) return;
    if (o->flags & 1) return;
    if (!(o->field24 & 0x400000)) return;

    Container* c = &o->cont;
    int local = 0x36;
    Node* r = c->LowerBound(&local);
    Node* e = c->end_;
    if (r != e && r->key <= 0x36) {
        // keep r
    } else {
        r = e;
    }
    FinalCall(r->p4);
}