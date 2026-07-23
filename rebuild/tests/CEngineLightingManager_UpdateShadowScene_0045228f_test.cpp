#include <cstdio>

struct CEngineLightingManager {
    char pad[0x18];
    int state;
    char pad2[0x24-0x1c];
    int field24;
    void* field28;
};

static int g_prep = 0;
static int g_set_count = 0, g_put_count = 0;
static int g_set_v = 0, g_put_v = 0;
static void* g_set_this = 0;
static void* g_put_this = 0;

struct CShadowSceneA {
    void Set(int v) { g_set_count++; g_set_v = v; g_set_this = this; }
};
struct CShadowSceneB {
    void Put(int v) { g_put_count++; g_put_v = v; g_put_this = this; }
};

void __stdcall PrepShadow(int tag) { g_prep = tag; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, int* p)
{
    PrepShadow(0x122d70e);
    switch (self->state) {
    case 2:
        ((CShadowSceneB*)p)->Put(self->field24);
        break;
    case 3:
        ((CShadowSceneA*)self->field28)->Set(*p);
        break;
    }
}

int main() {
    CEngineLightingManager m;
    m.field24 = 77;
    CShadowSceneA a3;
    m.field28 = &a3;
    int arg = 999;
    CShadowSceneB b2;

    /* state 2 -> ((B*)p)->Put(field24) : this=p, v=field24 */
    m.state = 2; g_prep=0; g_set_count=g_put_count=0; g_put_v=-1; g_put_this=0;
    CEngineLightingManager_UpdateShadowScene(&m, 0, (int*)&b2);
    if (!(g_prep==0x122d70e && g_put_count==1 && g_set_count==0 && g_put_v==77 && g_put_this==(void*)&b2)) { std::printf("FAIL state2\n"); return 1; }

    /* state 3 -> ((A*)field28)->Set(*p) : this=field28, v=*p */
    m.state = 3; g_prep=0; g_set_count=g_put_count=0; g_set_v=-1; g_set_this=0;
    CEngineLightingManager_UpdateShadowScene(&m, 0, &arg);
    if (!(g_prep==0x122d70e && g_set_count==1 && g_put_count==0 && g_set_v==999 && g_set_this==(void*)&a3)) { std::printf("FAIL state3\n"); return 1; }

    /* other state -> nothing */
    m.state = 9; g_prep=0; g_set_count=g_put_count=0;
    CEngineLightingManager_UpdateShadowScene(&m, 0, &arg);
    if (!(g_prep==0x122d70e && g_set_count==0 && g_put_count==0)) { std::printf("FAIL state9\n"); return 1; }

    std::printf("CEngineLightingManager_0045228f_TEST PASS\n");
    return 0;
}