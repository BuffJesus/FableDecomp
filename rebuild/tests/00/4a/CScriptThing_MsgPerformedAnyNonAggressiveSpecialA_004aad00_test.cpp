#include <cstdio>

struct CInner;
struct CInnerVtbl { char pad[0xa0]; bool (__fastcall *slot40)(CInner*); };
struct CInner { CInnerVtbl* vtbl; };
struct CScriptThing { void* pad0; CInner* inner; };

bool __fastcall CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility(CScriptThing* self)
{
    CInner* inner = self->inner;
    if (!inner)
        return false;
    return inner->vtbl->slot40(inner);
}

static bool __fastcall ret_true(CInner*) { return true; }

int main()
{
    CScriptThing a;
    a.pad0 = 0;
    a.inner = 0;
    if (CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility(&a) != false) {
        std::printf("FAIL null path\n");
        return 1;
    }

    CInnerVtbl vt;
    vt.slot40 = ret_true;
    CInner inner;
    inner.vtbl = &vt;
    CScriptThing b;
    b.pad0 = 0;
    b.inner = &inner;
    if (CScriptThing_MsgPerformedAnyNonAggressiveSpecialAbility(&b) != true) {
        std::printf("FAIL vcall path\n");
        return 1;
    }

    std::printf("CScriptThing_004aad00_TEST PASS\n");
    return 0;
}