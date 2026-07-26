#include <cstdio>

struct CPlayerGuiDef
{
    CPlayerGuiDef();
    int x;
};

static CPlayerGuiDef *g_playerGuiDef = 0;

CPlayerGuiDef::CPlayerGuiDef() { x = 0x1234; }

CPlayerGuiDef *CPlayerGui_PeekGuiDef()
{
    if (g_playerGuiDef == 0)
        g_playerGuiDef = new CPlayerGuiDef();
    return g_playerGuiDef;
}

int main()
{
    CPlayerGuiDef *a = CPlayerGui_PeekGuiDef();
    CPlayerGuiDef *b = CPlayerGui_PeekGuiDef();
    if (a != 0 && a == b && a->x == 0x1234)
    {
        std::printf("_global_00438570_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}