#include <cstdio>

static int g_a, g_b, g_mode;

struct CThingChecker {
    bool CanThingBeSeen(void* a, void* b, int mode);
};
bool CThingChecker::CanThingBeSeen(void* a, void* b, int mode)
{
    g_a = (int)(long)a;
    g_b = (int)(long)b;
    g_mode = mode;
    return mode == 2 && a != b;
}

struct CGameScriptInterface {
    bool CanThingBe_NearlySeen_ByOtherThing(void* thingA, void* thingB);
};

bool CGameScriptInterface::CanThingBe_NearlySeen_ByOtherThing(void* thingA, void* thingB)
{
    return ((CThingChecker*)this)->CanThingBeSeen(thingA, thingB, 2);
}

int main()
{
    CGameScriptInterface obj;
    int x = 11, y = 22;
    bool r = obj.CanThingBe_NearlySeen_ByOtherThing(&x, &y);
    if (!r || g_mode != 2 || g_a != (int)(long)&x || g_b != (int)(long)&y) {
        std::printf("CGameScriptInterface_0089cd30_TEST FAIL\n");
        return 1;
    }
    std::printf("CGameScriptInterface_0089cd30_TEST PASS\n");
    return 0;
}