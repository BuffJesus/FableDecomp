#include <cstdio>

int g_profile = 0;
int g_build = 0;
int g_apply = 0;
void* g_build_a = 0;
void* g_build_b = 0;
void* g_apply_self = 0;
void* g_apply_a = 0;

extern "C" void __stdcall Profile_Enter(int marker) { g_profile = marker; }

struct ShadowSceneLocal {
    int v;
    void Build(void* a, void* b) { g_build++; g_build_a = a; g_build_b = b; }
};

struct CShadowSceneTarget {
    void Apply(void* a) { g_apply++; g_apply_self = this; g_apply_a = a; }
};

struct CEngineLightingManager {
    char pad[0x18];
    int mode;      // +0x18
    char pad2[0x24 - 0x1c];
    void* p24;                 // +0x24
    CShadowSceneTarget* p28;   // +0x28
    void UpdateShadowScene(void* arg);
};

void CEngineLightingManager::UpdateShadowScene(void* arg)
{
    Profile_Enter(0x122d70e);
    ShadowSceneLocal loc;
    loc.v = 0;
    switch (this->mode) {
    case 2:
        loc.Build(this->p24, arg);
        break;
    case 3:
        this->p28->Apply(*(void**)arg);
        break;
    }
}

int main() {
    CEngineLightingManager obj;
    CShadowSceneTarget tgt;
    obj.p24 = (void*)0x1111;
    obj.p28 = &tgt;
    void* argslot = (void*)0x9999;
    void* argptr = &argslot;

    obj.mode = 2;
    g_profile = 0; g_build = 0; g_apply = 0;
    obj.UpdateShadowScene(argptr);
    if (g_profile != 0x122d70e) { std::printf("FAIL profile\n"); return 1; }
    if (g_build != 1 || g_apply != 0) { std::printf("FAIL case2\n"); return 1; }
    if (g_build_a != (void*)0x1111 || g_build_b != argptr) { std::printf("FAIL case2 args\n"); return 1; }

    obj.mode = 3;
    g_build = 0; g_apply = 0;
    obj.UpdateShadowScene(argptr);
    if (g_apply != 1 || g_build != 0) { std::printf("FAIL case3\n"); return 1; }
    if (g_apply_self != (void*)&tgt) { std::printf("FAIL case3 self\n"); return 1; }
    if (g_apply_a != argslot) { std::printf("FAIL case3 a\n"); return 1; }

    obj.mode = 5;
    g_build = 0; g_apply = 0;
    obj.UpdateShadowScene(argptr);
    if (g_build != 0 || g_apply != 0) { std::printf("FAIL default\n"); return 1; }

    std::printf("CEngineLightingManager_00452086_TEST PASS\n");
    return 0;
}