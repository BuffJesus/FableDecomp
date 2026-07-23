#include <cstdio>

static int g_a_calls = 0, g_b_calls = 0, g_setup = 0;
static int g_a_val = 0, g_b_arg = 0;

struct LMLocal { int v; void Fill(void* a, int arg) { (void)a; g_b_calls++; g_b_arg = arg; } };
struct LMTarget {
    void Handle(int val) { g_a_calls++; g_a_val = val; }
    void Setup(int tag) { (void)tag; g_setup++; }
};

struct CEngineLightingManager {
    char pad[0x18];
    int mode;
    char pad2[0x24-0x1c];
    void* p24;
    LMTarget* p28;
};

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, int* arg)
{
    LMLocal local;
    ((LMTarget*)self)->Setup(0x122d70e);
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Fill(self->p24, (int)arg);
        break;
    case 3:
        self->p28->Handle(*arg);
        break;
    }
}

int main() {
    CEngineLightingManager o;
    LMTarget tgt;
    int v = 777;
    o.p24=(void*)0x1; o.p28=&tgt;
    // mode 2
    g_a_calls=g_b_calls=0;
    o.mode = 2;
    CEngineLightingManager_UpdateShadowScene(&o, 0, &v);
    if (g_b_calls!=1 || g_a_calls!=0 || g_b_arg!=(int)&v) { std::printf("FAIL mode2\n"); return 1; }
    // mode 3
    g_a_calls=g_b_calls=0;
    o.mode = 3;
    CEngineLightingManager_UpdateShadowScene(&o, 0, &v);
    if (g_a_calls!=1 || g_b_calls!=0 || g_a_val!=777) { std::printf("FAIL mode3\n"); return 1; }
    // other
    g_a_calls=g_b_calls=0;
    o.mode = 5;
    CEngineLightingManager_UpdateShadowScene(&o, 0, &v);
    if (g_a_calls!=0 || g_b_calls!=0) { std::printf("FAIL other\n"); return 1; }
    std::printf("CEngineLightingManager_004521b2_TEST PASS\n");
    return 0;
}