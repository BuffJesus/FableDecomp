#include <cstdio>

struct CScriptThing;

struct CScriptThingVtbl {
    unsigned char _pad[0x12c];
    unsigned char (__fastcall *IsAlive)(CScriptThing* self);
};

struct CScriptThing {
    CScriptThingVtbl* vtbl;
    unsigned char aliveVal;
};

static unsigned char __fastcall stub_IsAlive(CScriptThing* self)
{
    return self->aliveVal;
}

bool __fastcall CScriptThing_IsDead(CScriptThing* self)
{
    return self->vtbl->IsAlive(self) == 0;
}

int main()
{
    CScriptThingVtbl vt;
    vt.IsAlive = stub_IsAlive;
    CScriptThing t;
    t.vtbl = &vt;

    t.aliveVal = 1;
    if (CScriptThing_IsDead(&t) != false) { std::printf("FAIL: alive nonzero should be not dead\n"); return 1; }
    t.aliveVal = 7;
    if (CScriptThing_IsDead(&t) != false) { std::printf("FAIL: alive 7 should be not dead\n"); return 1; }
    t.aliveVal = 0;
    if (CScriptThing_IsDead(&t) != true) { std::printf("FAIL: alive 0 should be dead\n"); return 1; }

    std::printf("CScriptThing_004aa890_TEST PASS\n");
    return 0;
}