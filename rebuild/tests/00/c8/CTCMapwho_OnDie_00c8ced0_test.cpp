#include <cstdio>

struct CTCMapwho { int cleaned; };

static int g_cleanupCalls = 0;
static int g_globalCalls = 0;

void __fastcall Mapwho_Cleanup(CTCMapwho* self) {
    g_cleanupCalls++;
    self->cleaned = 1;
}

typedef void (*GlobalDieFn)(CTCMapwho*);
static void myGlobalDie(CTCMapwho* p) { g_globalCalls++; (void)p; }
GlobalDieFn g_dieFn = myGlobalDie;

int __fastcall CTCMapwho_OnDie(CTCMapwho* self) {
    if (self) {
        Mapwho_Cleanup(self);
        g_dieFn(self);
    }
    return 0;
}

int main() {
    CTCMapwho m;
    m.cleaned = 0;
    int r = CTCMapwho_OnDie(&m);
    int r2 = CTCMapwho_OnDie(0);
    if (r == 0 && r2 == 0 && g_cleanupCalls == 1 && g_globalCalls == 1 && m.cleaned == 1) {
        printf("ONDIE_OK\n");
        return 0;
    }
    printf("ONDIE_FAIL\n");
    return 1;
}