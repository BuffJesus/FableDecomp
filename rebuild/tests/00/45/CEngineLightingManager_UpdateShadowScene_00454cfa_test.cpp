#include <cstdio>

struct CEngineLightingManager {
    char pad00[0x18];
    int mode;
    char pad1c[0x24-0x1c];
    void* obj24;
    void* obj28;
};

static int g_enter = 0;
static int g_a = 0;
static int g_a_arg = 0;
static int g_b = 0;

struct CShadowLocal {
    int v;
    void Build(void* obj24, int* param) { g_b = 1; }
};

struct CShadowTarget {
    void Emit(int arg) { g_a = 1; g_a_arg = arg; }
};

void __fastcall LM_Enter(CEngineLightingManager* self, void* edx, void* marker) { g_enter = (int)(long)marker; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, int* param)
{
    LM_Enter(self, edx, (void*)0x122d70e);
    CShadowLocal local;
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Build(self->obj24, param);
        break;
    case 3:
        ((CShadowTarget*)self->obj28)->Emit(*param);
        break;
    }
}

int main() {
    CEngineLightingManager o;
    CShadowTarget tgt;
    int p = 77;
    o.obj28 = &tgt;

    o.mode = 3; g_a = 0; g_b = 0; g_a_arg = 0;
    CEngineLightingManager_UpdateShadowScene(&o, 0, &p);
    if (g_enter != 0x122d70e || g_a != 1 || g_a_arg != 77 || g_b != 0) {
        std::printf("FAIL mode3\n"); return 1;
    }

    o.mode = 2; g_a = 0; g_b = 0;
    CEngineLightingManager_UpdateShadowScene(&o, 0, &p);
    if (g_a != 0 || g_b != 1) { std::printf("FAIL mode2\n"); return 1; }

    o.mode = 5; g_a = 0; g_b = 0;
    CEngineLightingManager_UpdateShadowScene(&o, 0, &p);
    if (g_a != 0 || g_b != 0) { std::printf("FAIL mode5\n"); return 1; }

    std::printf("CEngineLightingManager_00454cfa_TEST PASS\n");
    return 0;
}