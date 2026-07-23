#include <cstdio>

struct CResult {
    int flag;
    bool Method2(int n) { return flag == n; }
};
struct CSub {
    CResult* r;
    CResult* GetResult() { return r; }
};
struct CGlobal {
    char pad[0x1c];
    CSub* sub;
};

CGlobal* g_global;

int __fastcall CGameScriptInterface_IsHeroInTavernGame(void* self)
{
    (void)self;
    CResult* r = g_global->sub->GetResult();
    if (r && !r->Method2(0x1a))
        return 0;
    return 1;
}

int main()
{
    CSub sub_null; sub_null.r = 0;
    CGlobal g1; g1.sub = &sub_null;
    g_global = &g1;
    if (CGameScriptInterface_IsHeroInTavernGame(0) != 1) { std::printf("FAIL case1\n"); return 1; }

    CResult res_t; res_t.flag = 0x1a;
    CSub sub_t; sub_t.r = &res_t;
    CGlobal g2; g2.sub = &sub_t;
    g_global = &g2;
    if (CGameScriptInterface_IsHeroInTavernGame(0) != 1) { std::printf("FAIL case2\n"); return 1; }

    CResult res_f; res_f.flag = 5;
    CSub sub_f; sub_f.r = &res_f;
    CGlobal g3; g3.sub = &sub_f;
    g_global = &g3;
    if (CGameScriptInterface_IsHeroInTavernGame(0) != 0) { std::printf("FAIL case3\n"); return 1; }

    std::printf("CGameScriptInterface_008913c0_TEST PASS\n");
    return 0;
}