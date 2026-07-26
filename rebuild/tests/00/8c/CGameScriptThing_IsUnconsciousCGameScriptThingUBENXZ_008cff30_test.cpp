#include <cstdio>

struct CGameScriptThing;

bool __fastcall CGameScriptThing_IsUnconscious(const CGameScriptThing* self)
{
    (void)self;
    return false;
}

int main()
{
    if (CGameScriptThing_IsUnconscious((const CGameScriptThing*)0) == false)
    {
        std::printf("CGameScriptThing_008cff30_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}