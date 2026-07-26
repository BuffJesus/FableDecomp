#include <cstdio>

struct CThing { unsigned char pad[0x92]; };

struct CThingRef {
    CThing* target;
    CThing* Get();
};

CThing* CThingRef::Get() { return target; }

struct CGameScriptThing {
    char pad[0xc];
    CThingRef ref;
};

CThing* __fastcall CGameScriptThing_GetPThing(const CGameScriptThing* self)
{
    CThingRef* r = (CThingRef*)&self->ref;
    if (r->Get() != 0) {
        if ((*((unsigned char*)r->Get() + 0x91) & 1) == 0)
            return r->Get();
    }
    return 0;
}

int main()
{
    CThing thing;
    for (int i = 0; i < 0x92; ++i) thing.pad[i] = 0;

    CGameScriptThing g;
    g.ref.target = 0;

    if (CGameScriptThing_GetPThing(&g) != 0) { std::printf("FAIL null\n"); return 1; }

    g.ref.target = &thing;
    thing.pad[0x91] = 0;
    if (CGameScriptThing_GetPThing(&g) != &thing) { std::printf("FAIL clear\n"); return 1; }

    thing.pad[0x91] = 1;
    if (CGameScriptThing_GetPThing(&g) != 0) { std::printf("FAIL set\n"); return 1; }

    thing.pad[0x91] = 2;
    if (CGameScriptThing_GetPThing(&g) != &thing) { std::printf("FAIL bit1\n"); return 1; }

    std::printf("CGameScriptThing_008d0440_TEST PASS\n");
    return 0;
}