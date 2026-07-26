#include <cstdio>

const char* kTag = "shadow";

static int g_enter=0, g_a=0, g_b=0;

struct CShadowRenderer {
    void Update(void* scene){ (void)scene; g_b++; }
};

struct CLocalScene {
    void* p;
    void Fill(void* rendA, void* scene){ (void)rendA;(void)scene; g_a++; }
};

struct CEngineLightingManager {
    char pad0[0x18];
    int  mode;                 // +0x18
    char pad1[0x24-0x1c];
    void* rendA;               // +0x24
    CShadowRenderer* rendB;    // +0x28

    void Enter(const char* tag){ (void)tag; g_enter++; }
    void UpdateShadowScene(void** scene);
};

void CEngineLightingManager::UpdateShadowScene(void** scene)
{
    CLocalScene local;
    Enter(kTag);
    local.p = 0;
    switch (mode) {
    case 2:
        local.Fill(rendA, scene);
        break;
    case 3:
        rendB->Update(*scene);
        break;
    }
}

int main(){
    CEngineLightingManager mgr;
    CShadowRenderer rend;
    mgr.rendA = (void*)0x99;
    mgr.rendB = &rend;
    void* scenebuf = (void*)0x1234;
    void* scene = &scenebuf;

    mgr.mode = 2; g_enter=g_a=g_b=0;
    mgr.UpdateShadowScene((void**)&scene);
    if(!(g_enter==1 && g_a==1 && g_b==0)){ std::printf("FAIL mode2\n"); return 1; }

    mgr.mode = 3; g_enter=g_a=g_b=0;
    mgr.UpdateShadowScene((void**)&scene);
    if(!(g_enter==1 && g_a==0 && g_b==1)){ std::printf("FAIL mode3\n"); return 1; }

    mgr.mode = 5; g_enter=g_a=g_b=0;
    mgr.UpdateShadowScene((void**)&scene);
    if(!(g_enter==1 && g_a==0 && g_b==0)){ std::printf("FAIL mode-other\n"); return 1; }

    std::printf("CEngineLightingManager_0045290a_TEST PASS\n");
    return 0;
}