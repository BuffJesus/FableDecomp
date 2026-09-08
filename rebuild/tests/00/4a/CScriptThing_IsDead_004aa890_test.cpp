#include <cstdio>
#include "engine/CScriptThing.h"

struct CScriptThingVtbl {
    unsigned char _pad[0x12c];
    unsigned char (__fastcall *IsAlive)(CScriptThing* self);
};

static unsigned char g_aliveVal = 0;

static unsigned char __fastcall stub_IsAlive(CScriptThing* self)
{
    (void)self;
    return g_aliveVal;
}

bool __fastcall CScriptThing_IsDead(CScriptThing* self)
{
    return ((CScriptThingVtbl*)self->__vftable)->IsAlive(self) == 0;
}

int main()
{
    CScriptThingVtbl vt;
    vt.IsAlive = stub_IsAlive;
    CScriptThing t;
    t.__vftable = &vt;
    t.PImp_Data = 0;
    t.PImp_Info = 0;

    g_aliveVal = 1;
    if (CScriptThing_IsDead(&t) != false) { std::printf("FAIL: alive nonzero should be not dead\n"); return 1; }
    g_aliveVal = 7;
    if (CScriptThing_IsDead(&t) != false) { std::printf("FAIL: alive 7 should be not dead\n"); return 1; }
    g_aliveVal = 0;
    if (CScriptThing_IsDead(&t) != true) { std::printf("FAIL: alive 0 should be dead\n"); return 1; }

    std::printf("CScriptThing_004aa890_TEST PASS\n");
    return 0;
}
