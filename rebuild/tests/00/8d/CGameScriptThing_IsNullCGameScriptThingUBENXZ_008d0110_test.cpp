#include <cstdio>

struct SThing {
    void* p0;
    int   v4;
};
struct CGameScriptThing {
    char pad[0x20];
    SThing* pThing;
};

SThing* g_nullThing = 0;
static int g_cmp_ret = 0;
extern "C" int __fastcall Thing_Compare(void* a, void* b) { (void)a; (void)b; return g_cmp_ret; }

bool __fastcall CGameScriptThing_IsNull(const CGameScriptThing* self)
{
    SThing* a = self->pThing;
    SThing* b = g_nullThing;
    if (a == b)
        return true;
    if (a != 0 && b != 0 && a->v4 == b->v4)
        return (bool)(Thing_Compare(a->p0, b->p0) == 0);
    return false;
}

int main() {
    SThing nullT; nullT.p0 = (void*)0x1; nullT.v4 = 7;
    g_nullThing = &nullT;

    CGameScriptThing o;
    o.pThing = &nullT;
    if (CGameScriptThing_IsNull(&o) != true) { std::printf("FAIL case1\n"); return 1; }

    o.pThing = 0;
    if (CGameScriptThing_IsNull(&o) != false) { std::printf("FAIL case2\n"); return 1; }

    SThing other; other.p0 = (void*)0x2; other.v4 = 99;
    o.pThing = &other;
    if (CGameScriptThing_IsNull(&o) != false) { std::printf("FAIL case3\n"); return 1; }

    other.v4 = 7; g_cmp_ret = 0;
    if (CGameScriptThing_IsNull(&o) != true) { std::printf("FAIL case4\n"); return 1; }

    g_cmp_ret = 5;
    if (CGameScriptThing_IsNull(&o) != false) { std::printf("FAIL case5\n"); return 1; }

    g_nullThing = 0; o.pThing = &other;
    if (CGameScriptThing_IsNull(&o) != false) { std::printf("FAIL case6\n"); return 1; }

    std::printf("CGameScriptThing_008d0110_TEST PASS\n");
    return 0;
}