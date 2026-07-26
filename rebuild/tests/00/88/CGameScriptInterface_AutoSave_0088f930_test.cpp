#include <cstdio>

static int g_called = 0;
static int g_arg = -1;
static void* g_self = 0;

struct CMid {
    void Save(int a);
};

void CMid::Save(int a)
{
    g_called++;
    g_arg = a;
    g_self = (void*)this;
}

struct CGameScriptInterface {
    void* vt;
    CMid* mid;
};

void __fastcall CGameScriptInterface_AutoSave(CGameScriptInterface* self)
{
    self->mid->Save(0);
}

int main()
{
    CGameScriptInterface gsi;
    gsi.vt = 0;
    gsi.mid = (CMid*)0x12345678;
    CGameScriptInterface_AutoSave(&gsi);
    if (g_called == 1 && g_arg == 0 && g_self == (void*)0x12345678) {
        std::printf("CGameScriptInterface_0088f930_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL called=%d arg=%d\n", g_called, g_arg);
    return 1;
}