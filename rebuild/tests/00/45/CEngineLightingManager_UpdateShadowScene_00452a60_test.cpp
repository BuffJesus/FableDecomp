#include <cstdio>
#pragma optimize("s",on)

static int g_prologue = 0;
static int g_emit = 0, g_apply = 0;
static int g_last24 = 0, g_lastparam = 0, g_lastarg = 0;
static void* g_applyThis = 0;

struct CLocalSink {
    int slot;
    void Emit(int obj24, int* param) { g_emit++; g_last24 = obj24; g_lastparam = *param; }
};
struct CShadowObj {
    void Apply(int arg) { g_apply++; g_applyThis = this; g_lastarg = arg; }
};

struct CEngineLightingManager {
    char pad0[0x18];
    int mode;          // +0x18
    char pad1c[0x24-0x1c];
    CShadowObj* obj24; // +0x24
    CShadowObj* obj28; // +0x28
};

extern "C" void __stdcall LM_Prologue(int cookie) { g_prologue = cookie; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* /*edx*/, int* param)
{
    CLocalSink sink;
    LM_Prologue(0x122d70e);
    sink.slot = 0;
    switch (self->mode) {
    case 2:
        sink.Emit((int)self->obj24, param);
        break;
    case 3:
        self->obj28->Apply(*param);
        break;
    }
}

int main() {
    CEngineLightingManager m;
    CShadowObj o24, o28;
    m.mode = 2; m.obj24 = &o24; m.obj28 = &o28;
    int p = 0x555;
    CEngineLightingManager_UpdateShadowScene(&m, 0, &p);
    if (g_prologue != 0x122d70e) { printf("FAIL prologue\n"); return 1; }
    if (g_emit != 1 || g_last24 != (int)&o24 || g_lastparam != 0x555) { printf("FAIL mode2\n"); return 1; }

    m.mode = 3; g_prologue = 0;
    CEngineLightingManager_UpdateShadowScene(&m, 0, &p);
    if (g_apply != 1 || g_applyThis != &o28 || g_lastarg != 0x555) { printf("FAIL mode3\n"); return 1; }

    m.mode = 5; g_emit = g_apply = 0;
    CEngineLightingManager_UpdateShadowScene(&m, 0, &p);
    if (g_emit != 0 || g_apply != 0) { printf("FAIL mode-other\n"); return 1; }

    printf("CEngineLightingManager_00452a60_TEST PASS\n");
    return 0;
}