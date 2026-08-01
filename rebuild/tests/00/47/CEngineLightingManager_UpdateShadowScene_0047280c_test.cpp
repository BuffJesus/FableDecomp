#include <cstdio>

static int g_prof = 0;
static int g_c24 = 0, g_c28 = 0;
static void* g_self24 = 0;
static void* g_self28 = 0;
static void* g_arg24 = 0;
static void* g_arg28 = 0;

extern "C" void __stdcall Profile_Enter(int marker) { (void)marker; g_prof++; }
void __fastcall LM_ShadowApply24(void* self24, void* arg) { g_c24++; g_self24 = self24; g_arg24 = arg; }
void __fastcall LM_ShadowApply28(void* self28, void* arg) { g_c28++; g_self28 = self28; g_arg28 = arg; }

struct CEngineLightingManager {
    char pad[0x18];
    int mode;                  /* +0x18 */
    char pad2[0x24 - 0x1c];
    void* p24;                 /* +0x24 */
    void* p28;                 /* +0x28 */
};

void __fastcall CEngineLightingManager_UpdateShadowScene(
    CEngineLightingManager* self, void* edx, void* arg);

int main() {
    CEngineLightingManager o;
    int dummy24 = 24, dummy28 = 28;
    int argval = 0;
    void* argp = &argval;
    o.p24 = &dummy24;
    o.p28 = &dummy28;

    /* mode 2 -> p24 path */
    g_prof = g_c24 = g_c28 = 0;
    o.mode = 2;
    CEngineLightingManager_UpdateShadowScene(&o, 0, argp);
    if (g_prof != 1 || g_c24 != 1 || g_c28 != 0 ||
        g_self24 != (void*)&dummy24 || g_arg24 != argp) {
        std::printf("FAIL mode2\n"); return 1;
    }

    /* mode 3 -> p28 path */
    g_prof = g_c24 = g_c28 = 0;
    o.mode = 3;
    CEngineLightingManager_UpdateShadowScene(&o, 0, argp);
    if (g_prof != 1 || g_c28 != 1 || g_c24 != 0 ||
        g_self28 != (void*)&dummy28 || g_arg28 != argp) {
        std::printf("FAIL mode3\n"); return 1;
    }

    /* other mode -> no dispatch (profile still fires) */
    g_prof = g_c24 = g_c28 = 0;
    o.mode = 5;
    CEngineLightingManager_UpdateShadowScene(&o, 0, argp);
    if (g_prof != 1 || g_c24 != 0 || g_c28 != 0) {
        std::printf("FAIL other\n"); return 1;
    }

    std::printf("UPDATE_SHADOW_SCENE_47280C_TEST PASS\n");
    return 0;
}