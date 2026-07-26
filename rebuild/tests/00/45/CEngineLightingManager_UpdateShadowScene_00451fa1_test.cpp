#include <cstdio>

struct CEngineLightingManager {
    char pad[0x18];
    int  mode;
    char pad2[0x24-0x1c];
    void* obj24;
    void* obj28;
};

static int g_sceneApplyCalls = 0;
static int g_localApplyCalls = 0;
static int g_lastSceneArg = 0;

struct CScene {
    void __fastcall Apply(int a) { g_sceneApplyCalls++; g_lastSceneArg = a; }
};
struct CLocal {
    int v;
    void __fastcall Do(void* a, int* b) { (void)a; (void)b; g_localApplyCalls++; }
};

void __stdcall LightScope(const char* tag) { (void)tag; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, int* arg)
{
    (void)edx;
    LightScope((const char*)0x122d70e);
    CLocal local;
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Do(self->obj24, arg);
        break;
    case 3:
        ((CScene*)self->obj28)->Apply(*arg);
        break;
    }
}

int main()
{
    CEngineLightingManager mgr;
    int arg = 77;
    CScene scene;

    mgr.mode = 3; mgr.obj28 = &scene;
    g_sceneApplyCalls = 0; g_lastSceneArg = 0;
    CEngineLightingManager_UpdateShadowScene(&mgr, 0, &arg);
    if (g_sceneApplyCalls != 1 || g_lastSceneArg != 77) { std::printf("FAIL mode3\n"); return 1; }

    mgr.mode = 2; mgr.obj24 = (void*)0x9999;
    g_localApplyCalls = 0;
    CEngineLightingManager_UpdateShadowScene(&mgr, 0, &arg);
    if (g_localApplyCalls != 1) { std::printf("FAIL mode2\n"); return 1; }

    mgr.mode = 0;
    g_sceneApplyCalls = 0; g_localApplyCalls = 0;
    CEngineLightingManager_UpdateShadowScene(&mgr, 0, &arg);
    if (g_sceneApplyCalls != 0 || g_localApplyCalls != 0) { std::printf("FAIL mode0\n"); return 1; }

    std::printf("CEngineLightingManager_00451fa1_TEST PASS\n");
    return 0;
}