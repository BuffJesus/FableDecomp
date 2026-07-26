#include <cstdio>

struct RCObj {
    void** vtbl;
    int    refcnt;
    char   pad[0x68];
    int    field70;
};

static int g_released = 0;
void __fastcall RelStub(RCObj* o) { g_released++; }
static void* g_vt[2];

static void* g_cook_arg_a = 0;
static int g_cook_arg_b = 0;
bool __stdcall CookTest(void* a, int b) { g_cook_arg_a = a; g_cook_arg_b = b; return true; }

struct CTCVillage {
    char pad[0x94];
    RCObj* handle;
};

bool __fastcall CTCVillage_IsCookTime(CTCVillage* self)
{
    RCObj** slot = &self->handle;
    RCObj* a = *slot;
    if (a) a->refcnt++;
    RCObj* b = *slot;
    if (b) b->refcnt++;

    bool r = CookTest((char*)b + 0x64, a->field70);

    if (b) {
        if (--b->refcnt == 0)
            ((void(__fastcall*)(RCObj*))b->vtbl[1])(b);
    }
    if (a) {
        if (--a->refcnt == 0)
            ((void(__fastcall*)(RCObj*))a->vtbl[1])(a);
    }
    return r;
}

int main() {
    g_vt[1] = (void*)&RelStub;
    RCObj obj;
    obj.vtbl = g_vt;
    obj.refcnt = 5;
    obj.field70 = 1234;
    CTCVillage v;
    v.handle = &obj;

    bool r = CTCVillage_IsCookTime(&v);
    if (!r) { std::printf("FAIL: result false\n"); return 1; }
    if (obj.refcnt != 5) { std::printf("FAIL: refcnt %d\n", obj.refcnt); return 1; }
    if (g_released != 0) { std::printf("FAIL: released %d\n", g_released); return 1; }
    if (g_cook_arg_b != 1234) { std::printf("FAIL: arg_b %d\n", g_cook_arg_b); return 1; }
    if (g_cook_arg_a != (char*)&obj + 0x64) { std::printf("FAIL: arg_a\n"); return 1; }

    std::printf("CTCVillage_00658b20_TEST PASS\n");
    return 0;
}