#include <cstdio>

struct Obj28;

static int g_profile_hash = -1;
static int g_draw_value = -777;
static void* g_draw_self = 0;
static int g_fill_localv = -777;
static void* g_fill_self = 0;
static void* g_fill_obj24 = 0;
static void* g_fill_arg = 0;

struct Local4 {
    int v;
    void Fill(void* obj24, void* arg) { g_fill_self = this; g_fill_localv = this->v; g_fill_obj24 = obj24; g_fill_arg = arg; }
};
struct Obj28 {
    void Draw(int val) { g_draw_self = this; g_draw_value = val; }
};

struct CEngineLightingManager {
    char pad_18[0x18];
    int mode;
    char pad_1c[0x24-0x1c];
    void* obj24;
    Obj28* obj28;
};

extern "C" void __stdcall ProfileScope(unsigned int hash) { g_profile_hash = (int)hash; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* /*edx*/, void* arg)
{
    Local4 local;
    ProfileScope(0x122d70e);
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Fill(self->obj24, arg);
        break;
    case 3:
        self->obj28->Draw(*(int*)arg);
        break;
    }
}

int main() {
    CEngineLightingManager o;
    void* fakeObj24 = (void*)0x1111;
    Obj28 o28;
    o.obj24 = fakeObj24;
    o.obj28 = &o28;
    int argbuf = 0x9abc;
    void* arg = &argbuf;

    // mode == 2 -> Local4::Fill(obj24, arg), local zeroed first
    o.mode = 2;
    g_profile_hash = -1; g_fill_localv = -1; g_fill_obj24 = 0; g_fill_arg = 0;
    CEngineLightingManager_UpdateShadowScene(&o, 0, arg);
    if (g_profile_hash != 0x122d70e) { std::printf("FAIL: profile hash\n"); return 1; }
    if (g_fill_localv != 0) { std::printf("FAIL: local not zeroed\n"); return 1; }
    if (g_fill_obj24 != fakeObj24) { std::printf("FAIL: obj24\n"); return 1; }
    if (g_fill_arg != arg) { std::printf("FAIL: arg\n"); return 1; }

    // mode == 3 -> obj28->Draw(*arg)
    o.mode = 3;
    g_draw_value = -1; g_draw_self = 0;
    CEngineLightingManager_UpdateShadowScene(&o, 0, arg);
    if (g_draw_self != &o28) { std::printf("FAIL: obj28 self\n"); return 1; }
    if (g_draw_value != argbuf) { std::printf("FAIL: draw value\n"); return 1; }

    // mode == other -> nothing happens
    o.mode = 5;
    g_draw_value = 12345;
    CEngineLightingManager_UpdateShadowScene(&o, 0, arg);
    if (g_draw_value != 12345) { std::printf("FAIL: default path touched\n"); return 1; }

    std::printf("CEngineLightingManager_00451e59_TEST PASS\n");
    return 0;
}