#include <cstdio>

struct CWorld;
struct CWorldVtbl {
    void (__fastcall *slots[0x10])(CWorld*);
    void (__fastcall *onDisable)(CWorld*);
};
struct CWorld {
    CWorldVtbl* vtbl;
    char pad[0x24 - 4];
    bool f24;
};

void __fastcall SetMiniMapEnabled(CWorld* self, int edx, bool enabled);

static int g_calls = 0;
void __fastcall myDisable(CWorld*) { g_calls++; }

int main()
{
    CWorldVtbl vt;
    for (int i = 0; i < 0x10; i++) vt.slots[i] = 0;
    vt.onDisable = myDisable;

    CWorld w;
    w.vtbl = &vt;
    w.f24 = false;

    SetMiniMapEnabled(&w, 0, false);
    bool ok = (w.f24 == false && g_calls == 0);

    SetMiniMapEnabled(&w, 0, true);
    ok = ok && (w.f24 == true && g_calls == 0);

    SetMiniMapEnabled(&w, 0, true);
    ok = ok && (w.f24 == true && g_calls == 0);

    SetMiniMapEnabled(&w, 0, false);
    ok = ok && (w.f24 == false && g_calls == 1);

    SetMiniMapEnabled(&w, 0, false);
    ok = ok && (w.f24 == false && g_calls == 1);

    if (ok) printf("00991600_TEST PASS\n");
    else printf("FAIL f24=%d calls=%d\n", (int)w.f24, g_calls);
    return 0;
}