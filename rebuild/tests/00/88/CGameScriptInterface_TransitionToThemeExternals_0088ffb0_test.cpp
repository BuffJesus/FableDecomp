#include <cstdio>

static int g_count = 0;
static int g_method_calls = 0;
static int g_last_c = -1;
static int g_last_d = -1;

struct Thing {
    int count() { return g_count; }
};

struct Inner {
    void method(int a, int b, int c, int d) {
        g_method_calls++; g_last_c = c; g_last_d = d;
        (void)a; (void)b;
    }
};

struct CMid {
    unsigned char pad[0x18];
    Inner* p18;
};

struct CGameScriptInterface {
    void* vt;
    CMid* mid;
};

static Thing g_theme;
extern "C" Thing* __stdcall GetTheme(int arg1) { (void)arg1; return &g_theme; }

void __fastcall CGameScriptInterface_TransitionToThemeExternals(CGameScriptInterface* self, int edx_unused, int arg1, int arg2)
{
    Thing* t = GetTheme(arg1);
    int n = t->count();
    if (n > 0) {
        self->mid->p18->method(0, 1, n, arg2);
    }
}

int main() {
    Inner inner;
    CMid theMid; theMid.p18 = &inner;
    CGameScriptInterface iface; iface.mid = &theMid;

    g_count = 0; g_method_calls = 0;
    CGameScriptInterface_TransitionToThemeExternals(&iface, 0, 5, 42);
    if (g_method_calls != 0) { std::printf("FAIL: called when n<=0\n"); return 1; }

    g_count = 7; g_method_calls = 0;
    CGameScriptInterface_TransitionToThemeExternals(&iface, 0, 5, 42);
    if (g_method_calls != 1 || g_last_c != 7 || g_last_d != 42) {
        std::printf("FAIL: bad args c=%d d=%d calls=%d\n", g_last_c, g_last_d, g_method_calls);
        return 1;
    }

    std::printf("CGameScriptInterface_0088ffb0_TEST PASS\n");
    return 0;
}