#include <cstdio>

struct Sub { void** vtbl; };
struct CScriptThing { void* pad0; Sub* sub; };

typedef bool (__fastcall *SubFn)(Sub* self);

bool __fastcall CScriptThing_MsgPerformedAnyAggressiveSpecialAbility(CScriptThing* self)
{
    Sub* s = self->sub;
    if (!s)
        return false;
    return ((SubFn)(*(void***)s)[0x9c / 4])(s);
}

static bool __fastcall vfn(Sub* s) { (void)s; return true; }

int main()
{
    // null sub -> false
    CScriptThing a;
    a.pad0 = 0; a.sub = 0;
    if (CScriptThing_MsgPerformedAnyAggressiveSpecialAbility(&a) != false) { std::printf("FAIL null\n"); return 1; }

    // non-null sub -> virtual call
    void* vt[40];
    for (int i = 0; i < 40; ++i) vt[i] = 0;
    vt[0x9c / 4] = (void*)vfn;
    Sub s; s.vtbl = (void**)vt;
    CScriptThing b; b.pad0 = 0; b.sub = &s;
    if (CScriptThing_MsgPerformedAnyAggressiveSpecialAbility(&b) != true) { std::printf("FAIL vcall\n"); return 1; }

    std::printf("CScriptThing_004aace0_TEST PASS\n");
    return 0;
}