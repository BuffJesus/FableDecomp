#include <cstdio>

int g_descObj = 0;

int g_enterCalls = 0;
void* g_enterThis = 0;
void* g_enterArg = 0;

int g_t2Calls = 0;
void* g_t2This = 0;
void* g_t2A = 0;
void* g_t2B = 0;

int g_t3Calls = 0;
void* g_t3This = 0;
void* g_t3A = 0;

struct CTarget2 {
    void Draw(void* a, void* b) { g_t2Calls++; g_t2This = this; g_t2A = a; g_t2B = b; }
};
struct CTarget3 {
    void Draw(void* a) { g_t3Calls++; g_t3This = this; g_t3A = a; }
};

struct CEngineLightingManager {
    char pad[0x18];
    int mode;
    char pad2[0x24 - 0x1c];
    void* obj24;
    void* obj28;
    void Enter(void* d) { g_enterCalls++; g_enterThis = this; g_enterArg = d; }
};

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, int edx, void** param)
{
    int st;
    self->Enter(&g_descObj);
    st = 0;
    switch (self->mode) {
    case 2:
        ((CTarget2*)&st)->Draw(self->obj24, param);
        break;
    case 3:
        ((CTarget3*)self->obj28)->Draw(*param);
        break;
    }
}

int main() {
    CEngineLightingManager mgr;
    int o24 = 24, o28 = 28;
    mgr.obj24 = &o24;
    mgr.obj28 = &o28;
    void* paramVal = (void*)0xABCDEF01;
    void* slot = paramVal;

    // mode 2
    g_enterCalls = g_t2Calls = g_t3Calls = 0;
    mgr.mode = 2;
    CEngineLightingManager_UpdateShadowScene(&mgr, 0, &slot);
    if (g_enterCalls != 1) { std::printf("FAIL enter2\n"); return 1; }
    if (g_enterThis != &mgr) { std::printf("FAIL enterthis\n"); return 1; }
    if (g_enterArg != &g_descObj) { std::printf("FAIL enterarg\n"); return 1; }
    if (g_t2Calls != 1 || g_t3Calls != 0) { std::printf("FAIL disp2\n"); return 1; }
    if (g_t2A != &o24) { std::printf("FAIL t2a\n"); return 1; }
    if (g_t2B != &slot) { std::printf("FAIL t2b (param passed by pointer)\n"); return 1; }

    // mode 3
    g_enterCalls = g_t2Calls = g_t3Calls = 0;
    mgr.mode = 3;
    CEngineLightingManager_UpdateShadowScene(&mgr, 0, &slot);
    if (g_enterCalls != 1) { std::printf("FAIL enter3\n"); return 1; }
    if (g_t3Calls != 1 || g_t2Calls != 0) { std::printf("FAIL disp3\n"); return 1; }
    if (g_t3This != &o28) { std::printf("FAIL t3this\n"); return 1; }
    if (g_t3A != paramVal) { std::printf("FAIL t3a (should be *param)\n"); return 1; }

    // other mode
    g_enterCalls = g_t2Calls = g_t3Calls = 0;
    mgr.mode = 7;
    CEngineLightingManager_UpdateShadowScene(&mgr, 0, &slot);
    if (g_enterCalls != 1 || g_t2Calls != 0 || g_t3Calls != 0) { std::printf("FAIL other\n"); return 1; }

    std::printf("CEngineLightingManager_00456b7d_TEST PASS\n");
    return 0;
}