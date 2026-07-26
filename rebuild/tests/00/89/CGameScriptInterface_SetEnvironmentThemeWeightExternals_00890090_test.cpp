#include <cstdio>

struct Inner {
    int calls; int lastArg; int lastCount;
    void Do(int a, int b, int count, int arg) {
        calls++; lastArg = arg; lastCount = count;
    }
};
struct CMid { char pad[0x18]; Inner* p18; };
struct CGameScriptInterface { void* vt; CMid* mid; };

static Inner g_inner = {0,0,0};
static int g_countReturn = 0;
static int g_resolveArg = -1;

extern "C" void* __stdcall Resolve(int a) { g_resolveArg = a; return (void*)0x1234; }
extern "C" int __fastcall CountOf(void* self) { return g_countReturn; }

void __fastcall CGameScriptInterface_SetEnvironmentThemeWeightExternals(CGameScriptInterface* self, int edx, int arg1, int arg2)
{
    void* r = Resolve(arg1);
    int count = CountOf(r);
    if (count > 0) {
        self->mid->p18->Do(0, 1, count, arg2);
    }
}

int main() {
    CMid mnode; mnode.p18 = &g_inner;
    CGameScriptInterface obj; obj.vt = 0; obj.mid = &mnode;

    // count <= 0 : no DoThing
    g_inner.calls = 0; g_countReturn = 0;
    CGameScriptInterface_SetEnvironmentThemeWeightExternals(&obj, 0, 77, 999);
    if (g_resolveArg != 77) { std::printf("FAIL resolve arg\n"); return 1; }
    if (g_inner.calls != 0) { std::printf("FAIL should skip\n"); return 1; }

    // count > 0 : DoThing called with arg2 and count
    g_inner.calls = 0; g_countReturn = 5;
    CGameScriptInterface_SetEnvironmentThemeWeightExternals(&obj, 0, 42, 999);
    if (g_inner.calls != 1) { std::printf("FAIL should call\n"); return 1; }
    if (g_inner.lastArg != 999) { std::printf("FAIL arg passthrough\n"); return 1; }
    if (g_inner.lastCount != 5) { std::printf("FAIL count passthrough\n"); return 1; }

    std::printf("CGameScriptInterface_00890090_TEST PASS\n");
    return 0;
}