#include <cstdio>

struct CObjA { void Do(int v); };
struct CFreeTgt { void Do(void* a24, void* arg); };

struct CEngineLightingManager {
    char pad0[0x18];
    int mode;
    char pad1c[0x24-0x1c];
    CFreeTgt* obj24;
    CObjA* obj28;
};

static int g_prof = 0;
static int g_free = 0;
static int g_obja = 0;
static int g_obja_v = 0;

void __stdcall ProfScope(const char* tag) { (void)tag; g_prof++; }
void CObjA::Do(int v) { g_obja++; g_obja_v = v; }
void CFreeTgt::Do(void* a24, void* arg) { (void)a24; (void)arg; g_free++; *(int*)this = 7; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, int edxpad, void* arg)
{
    int local;
    ProfScope((const char*)0x122d70e);
    local = 0;
    switch (self->mode) {
    case 2:
        ((CFreeTgt*)&local)->Do(self->obj24, arg);
        break;
    case 3:
        self->obj28->Do(*(int*)arg);
        break;
    }
}

static CObjA g_a;
static CFreeTgt g_f;

int main() {
    CEngineLightingManager m;
    int arg = 42;
    m.obj28 = &g_a; m.obj24 = &g_f;
    m.mode = 3; g_obja = 0; g_obja_v = 0;
    CEngineLightingManager_UpdateShadowScene(&m, 0, &arg);
    if (g_obja != 1 || g_obja_v != 42) { std::printf("FAIL mode3\n"); return 1; }
    m.mode = 2; g_free = 0;
    CEngineLightingManager_UpdateShadowScene(&m, 0, &arg);
    if (g_free != 1) { std::printf("FAIL mode2\n"); return 1; }
    m.mode = 5; g_free = 0; g_obja = 0;
    CEngineLightingManager_UpdateShadowScene(&m, 0, &arg);
    if (g_free != 0 || g_obja != 0) { std::printf("FAIL modeother\n"); return 1; }
    std::printf("CEngineLightingManager_00455712_TEST PASS\n");
    return 0;
}