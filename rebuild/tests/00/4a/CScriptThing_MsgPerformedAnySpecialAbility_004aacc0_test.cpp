#include <cstdio>

struct Target;
struct TargetVtbl {
    char pad[0x98];
    bool (__fastcall *slot)(Target*);
};
struct Target {
    TargetVtbl* vtbl;
};
struct CScriptThing {
    void* pad0;
    Target* m_target;
};

bool __fastcall CScriptThing_MsgPerformedAnySpecialAbility(CScriptThing* self)
{
    Target* t = self->m_target;
    if (t == 0)
        return false;
    return t->vtbl->slot(t);
}

static bool __fastcall ret_true(Target*) { return true; }
static bool __fastcall ret_false(Target*) { return false; }

int main()
{
    // null target -> false
    CScriptThing a;
    a.pad0 = 0;
    a.m_target = 0;
    if (CScriptThing_MsgPerformedAnySpecialAbility(&a) != false) {
        std::printf("FAIL null\n");
        return 1;
    }

    // non-null target, vtable slot returns true
    TargetVtbl vt_true;
    vt_true.slot = ret_true;
    Target t_true;
    t_true.vtbl = &vt_true;
    CScriptThing b;
    b.pad0 = 0;
    b.m_target = &t_true;
    if (CScriptThing_MsgPerformedAnySpecialAbility(&b) != true) {
        std::printf("FAIL true\n");
        return 1;
    }

    // non-null target, vtable slot returns false
    TargetVtbl vt_false;
    vt_false.slot = ret_false;
    Target t_false;
    t_false.vtbl = &vt_false;
    CScriptThing c;
    c.pad0 = 0;
    c.m_target = &t_false;
    if (CScriptThing_MsgPerformedAnySpecialAbility(&c) != false) {
        std::printf("FAIL false\n");
        return 1;
    }

    std::printf("CScriptThing_004aacc0_TEST PASS\n");
    return 0;
}