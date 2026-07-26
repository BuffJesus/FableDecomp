#include <cstdio>
static int g_called = 0;
static int g_a1 = 0, g_a2 = 0, g_zero = -1;
static void* g_self = 0;
struct Target {
    void Show(int arg1, int arg2, int zero);
};
struct Mgr {
    unsigned char _pad[0x1c8];
    Target* target; // +0x1c8
};
void Target::Show(int arg1, int arg2, int zero)
{
    g_called = 1; g_self = this; g_a1 = arg1; g_a2 = arg2; g_zero = zero;
}
Mgr* g_mgr;

void __stdcall CGameScriptInterface_DisplayMiniGameInfo(int arg1, int arg2)
{
    g_mgr->target->Show(arg1, arg2, 0);
}

int main()
{
    static Mgr mgr;
    static Target tgt;
    mgr.target = &tgt;
    g_mgr = &mgr;
    CGameScriptInterface_DisplayMiniGameInfo(111, 222);
    if (g_called == 1 && g_self == &tgt && g_a1 == 111 && g_a2 == 222 && g_zero == 0) {
        std::printf("CGameScriptInterface_00891c60_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL called=%d a1=%d a2=%d zero=%d\n", g_called, g_a1, g_a2, g_zero);
    return 1;
}