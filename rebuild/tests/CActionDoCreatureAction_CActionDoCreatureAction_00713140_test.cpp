#include <cstdio>

struct CActionDoCreatureAction {
    void* vtbl;
    int   f4;
    char  f8;
    char  f9;
    int   fc;
    char  f10;
    char  pad[3];
    int   f14;
    int   f18;
};

static int g_base_called = 0;
static int g_sub_called = 0;
static void* g_sub_arg = 0;
static void* g_sub_self = 0;

static int g_vtbl_storage;
static void* const g_vtbl_1265838_ptr = &g_vtbl_storage;

void base_ctor(void* self){ g_base_called = 1; }

struct Sub {
    int a;
    int b;
    void Init(void* arg){ g_sub_called = 1; g_sub_self = this; g_sub_arg = arg; }
};

typedef void* (*VFn)(void* self);
static void* g_vfn_ret = (void*)0xABCD;
static void* virt(void* self){ return g_vfn_ret; }

static void* fake_vtbl[16];

CActionDoCreatureAction* build(CActionDoCreatureAction* self, void* pv)
{
    base_ctor(self);
    self->f4 = 0;
    self->f8 = 0;
    self->f9 = 0;
    self->vtbl = g_vtbl_1265838_ptr;
    int* p = &self->f14;
    p[0] = 0;
    p[1] = 0;
    self->fc = 0;
    self->f10 = 0;
    void* r = (*(VFn*)((char*)*(void**)pv + 0x2c))(pv);
    ((Sub*)&self->f14)->Init(r);
    return self;
}

int main(){
    fake_vtbl[11] = (void*)&virt;
    void* pv = &fake_vtbl;
    CActionDoCreatureAction obj;
    obj.vtbl = (void*)0xDEAD;
    obj.f4 = 123;
    CActionDoCreatureAction* ret = build(&obj, &pv);
    if (ret != &obj) { std::printf("FAIL: return\n"); return 1; }
    if (!g_base_called) { std::printf("FAIL: base\n"); return 1; }
    if (obj.f4 != 0 || obj.f8 != 0 || obj.f9 != 0 || obj.fc != 0 || obj.f10 != 0) { std::printf("FAIL: zero fields\n"); return 1; }
    if (obj.vtbl != g_vtbl_1265838_ptr) { std::printf("FAIL: vtbl\n"); return 1; }
    if (obj.f14 != 0 || obj.f18 != 0) { std::printf("FAIL: f14/f18 zeroed before sub\n"); return 1; }
    if (!g_sub_called) { std::printf("FAIL: sub not called\n"); return 1; }
    if (g_sub_self != &obj.f14) { std::printf("FAIL: sub self\n"); return 1; }
    if (g_sub_arg != (void*)0xABCD) { std::printf("FAIL: sub arg\n"); return 1; }
    std::printf("CActionDoCreatureAction_00713140_TEST PASS\n");
    return 0;
}