#include <cstdio>

static int g_prof = 0;
static int g_apply = 0, g_push = 0;
static void* g_apply_this = 0;
static void* g_apply_a = 0;
static void* g_apply_b = 0;
static void* g_push_this = 0;
static void* g_push_v = 0;

extern "C" void __stdcall Profile_Enter(int marker) { (void)marker; g_prof++; }

struct ShadowGuard {
    void* slot;
    void Apply(void* a, void* b);
};
void ShadowGuard::Apply(void* a, void* b) {
    g_apply++; g_apply_this = this; g_apply_a = a; g_apply_b = b;
}

struct ShadowSink {
    void Push(void* v);
};
void ShadowSink::Push(void* v) {
    g_push++; g_push_this = this; g_push_v = v;
}

struct CEngineLightingManager {
    char   pad0[0x18];
    int    mode;
    char   pad1[0x24 - 0x1c];
    void*  p24;
    ShadowSink* p28;

    void UpdateShadowScene(void* arg);
};

int main() {
    CEngineLightingManager o;
    ShadowSink sink;
    int slot24 = 24;
    void* argslot = (void*)0xDEAD;
    void* arg = &argslot;

    o.p24 = &slot24;
    o.p28 = &sink;

    /* case 2 -> Apply(p24, arg) on a stack guard */
    g_prof = g_apply = g_push = 0;
    o.mode = 2;
    o.UpdateShadowScene(arg);
    if (g_prof != 1 || g_apply != 1 || g_push != 0 ||
        g_apply_a != (void*)&slot24 || g_apply_b != arg) {
        std::printf("FAIL case2\n"); return 1;
    }

    /* case 3 -> p28->Push(*arg) */
    g_prof = g_apply = g_push = 0;
    o.mode = 3;
    o.UpdateShadowScene(arg);
    if (g_prof != 1 || g_push != 1 || g_apply != 0 ||
        g_push_this != (void*)&sink || g_push_v != argslot) {
        std::printf("FAIL case3\n"); return 1;
    }

    /* other mode -> no dispatch, profiler still fires */
    g_prof = g_apply = g_push = 0;
    o.mode = 7;
    o.UpdateShadowScene(arg);
    if (g_prof != 1 || g_apply != 0 || g_push != 0) {
        std::printf("FAIL other\n"); return 1;
    }

    std::printf("UPDATE_SHADOW_SCENE_479187_TEST PASS\n");
    return 0;
}