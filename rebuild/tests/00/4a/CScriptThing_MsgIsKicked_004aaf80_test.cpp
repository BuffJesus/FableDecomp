#include <cstdio>
struct CInner;
struct CInnerVtbl { char pad[0xf0]; bool (__fastcall *IsKicked)(CInner*); };
struct CInner { CInnerVtbl* vtbl; };
struct CScriptThing { void* pad0; CInner* inner; };

bool __fastcall CScriptThing_MsgIsKicked(CScriptThing* self)
{
    CInner* p = self->inner;
    if (!p)
        return false;
    return p->vtbl->IsKicked(p);
}

static bool __fastcall stub_true(CInner*) { return true; }

int main()
{
    CScriptThing t0; t0.pad0 = 0; t0.inner = 0;
    if (CScriptThing_MsgIsKicked(&t0) != false) { std::printf("FAIL null\n"); return 1; }

    CInnerVtbl vt; vt.IsKicked = stub_true;
    CInner obj; obj.vtbl = &vt;
    CScriptThing t1; t1.pad0 = 0; t1.inner = &obj;
    if (CScriptThing_MsgIsKicked(&t1) != true) { std::printf("FAIL virt\n"); return 1; }

    std::printf("CScriptThing_004aaf80_TEST PASS\n");
    return 0;
}