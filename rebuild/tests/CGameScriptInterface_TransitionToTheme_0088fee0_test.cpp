#include <cstdio>

struct Inner {
    int calls;
    int lastN;
    int lastArg2;
    void Apply(int one, int n, int arg2);
};
struct CMid { unsigned char pad[0x18]; Inner* p18; };
struct CGameScriptInterface { void* vt; CMid* mid; };

static int g_resolveArg = 0;
static int g_count = 0;

void Inner::Apply(int one, int n, int arg2) {
    calls = calls + 1;
    lastN = n;
    lastArg2 = arg2;
    (void)one;
}

void* __stdcall Resolve(int a) { g_resolveArg = a; return (void*)(long)a; }
int CountOf(void* self) { return g_count; }

void CGameScriptInterface_TransitionToTheme(CGameScriptInterface* self, int arg1, int arg2)
{
    void* r = Resolve(arg1);
    int n = CountOf(r);
    if (n > 0) {
        self->mid->p18->Apply(1, n, arg2);
    }
}

int main()
{
    Inner inner;
    inner.calls = 0;
    inner.lastN = 0;
    inner.lastArg2 = 0;
    CMid midObj;
    midObj.p18 = &inner;
    CGameScriptInterface gsi;
    gsi.mid = &midObj;

    g_count = 0;
    CGameScriptInterface_TransitionToTheme(&gsi, 42, 99);
    if (inner.calls != 0) { std::printf("FAIL: called when n<=0\n"); return 1; }
    if (g_resolveArg != 42) { std::printf("FAIL: resolve arg\n"); return 1; }

    g_count = 5;
    CGameScriptInterface_TransitionToTheme(&gsi, 7, 88);
    if (inner.calls != 1) { std::printf("FAIL: not called\n"); return 1; }
    if (inner.lastN != 5) { std::printf("FAIL: n value\n"); return 1; }
    if (inner.lastArg2 != 88) { std::printf("FAIL: arg2 value\n"); return 1; }
    if (g_resolveArg != 7) { std::printf("FAIL: resolve arg2\n"); return 1; }

    std::printf("CGameScriptInterface_0088fee0_TEST PASS\n");
    return 0;
}