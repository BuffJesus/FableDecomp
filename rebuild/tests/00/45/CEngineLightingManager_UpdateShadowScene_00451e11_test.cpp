#include <cstdio>

static int g_log = 0;

struct LocalObj {
    char pad[16];
    LocalObj();
    void apply24(void* fld, int arg);
    void apply28(void* fld, int arg);
};
LocalObj::LocalObj() { g_log += 1000; }
void LocalObj::apply24(void* fld, int arg) { g_log += 24; }
void LocalObj::apply28(void* fld, int arg) { g_log += 28; }

void __stdcall ProfileEnter(int tag) { g_log += tag ? 1 : 0; }

struct CEngineLightingManager {
    char pad[0x18];
    int mode;
    char pad2[0x24 - 0x1c];
    void* p24;
    void* p28;
};

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, int, int arg)
{
    ProfileEnter(0x122d70e);
    LocalObj local;
    switch (self->mode) {
    case 2: local.apply24(self->p24, arg); break;
    case 3: local.apply28(self->p28, arg); break;
    }
}

int main() {
    CEngineLightingManager mgr;
    for (int i = 0; i < (int)sizeof(mgr); i++) ((char*)&mgr)[i] = 0;

    mgr.mode = 2; g_log = 0;
    CEngineLightingManager_UpdateShadowScene(&mgr, 0, 7);
    if (g_log != 1000 + 1 + 24) { std::printf("FAIL mode2 %d\n", g_log); return 1; }

    mgr.mode = 3; g_log = 0;
    CEngineLightingManager_UpdateShadowScene(&mgr, 0, 7);
    if (g_log != 1000 + 1 + 28) { std::printf("FAIL mode3 %d\n", g_log); return 1; }

    mgr.mode = 5; g_log = 0;
    CEngineLightingManager_UpdateShadowScene(&mgr, 0, 7);
    if (g_log != 1000 + 1) { std::printf("FAIL modeDefault %d\n", g_log); return 1; }

    std::printf("CEngineLightingManager_00451e11_TEST PASS\n");
    return 0;
}