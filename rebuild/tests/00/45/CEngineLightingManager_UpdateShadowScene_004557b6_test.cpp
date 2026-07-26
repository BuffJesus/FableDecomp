#include <cstdio>

struct CEngineLightingManager {
    char pad[0x18];
    int mode;      // +0x18
    char pad2[0x24-0x1c];
    void* obj24;   // +0x24
    void* obj28;   // +0x28
};

static int g_which = 0;
static void* g_seen_arg = 0;

void __stdcall EHGuard(int cookie) { (void)cookie; }
struct Obj28 {
    void Handle28(void* arg) { g_which = 28; g_seen_arg = arg; }
};

struct LocalHelper {
    void Handle24(void* obj24, void* arg) { g_which = 24; g_seen_arg = arg; (void)obj24; }
};

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, void** arg)
{
    int local;
    EHGuard(0x122d70e);
    local = 0;
    switch (self->mode) {
    case 2:
        ((LocalHelper*)&local)->Handle24(self->obj24, arg);
        break;
    case 3:
        ((Obj28*)self->obj28)->Handle28(*arg);
        break;
    }
}

int main()
{
    CEngineLightingManager m;
    for (int i=0;i<(int)sizeof(m);++i) ((char*)&m)[i]=0;
    void* dummy = (void*)0x1234;
    void* argv = &dummy;
    Obj28 o28;
    m.obj28 = &o28;
    m.obj24 = (void*)0x5678;

    m.mode = 2;
    g_which = 0;
    CEngineLightingManager_UpdateShadowScene(&m, 0, (void**)&argv);
    if (g_which != 24) { std::printf("FAIL mode2 which=%d\n", g_which); return 1; }

    m.mode = 3;
    g_which = 0;
    CEngineLightingManager_UpdateShadowScene(&m, 0, (void**)&argv);
    if (g_which != 28) { std::printf("FAIL mode3 which=%d\n", g_which); return 1; }
    if (g_seen_arg != argv) { std::printf("FAIL mode3 arg\n"); return 1; }

    m.mode = 5;
    g_which = 0;
    CEngineLightingManager_UpdateShadowScene(&m, 0, (void**)&argv);
    if (g_which != 0) { std::printf("FAIL default which=%d\n", g_which); return 1; }

    std::printf("CEngineLightingManager_004557b6_TEST PASS\n");
    return 0;
}