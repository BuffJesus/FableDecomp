#include <cstdio>
struct Inner;
struct InnerVtbl {
    void (__fastcall *slot0)(Inner*);
    void (__fastcall *slot1)(Inner*);
};
struct Inner {
    InnerVtbl* vtbl;
    long refcount;
    char pad2[0x58 - 8];
    bool flag;
};
struct CTCWeapon {
    char pad[0x68];
    Inner* inner;
};
static int g_released = 0;
void __fastcall rel(Inner* p) { g_released = 1; }
bool __fastcall CTCWeapon_IsAnalogueLoading(CTCWeapon* self)
{
    Inner* p = self->inner;
    if (p) ++p->refcount;
    bool v = p->flag;
    if (--p->refcount == 0) {
        p->vtbl->slot1(p);
    }
    return v;
}
int main()
{
    InnerVtbl vt; vt.slot0 = 0; vt.slot1 = rel;
    Inner inst; inst.vtbl = &vt; inst.refcount = 5; inst.flag = 1;
    CTCWeapon w; w.inner = &inst;
    bool r = CTCWeapon_IsAnalogueLoading(&w);
    if (r != true) { std::printf("FAIL result\n"); return 1; }
    if (inst.refcount != 5) { std::printf("FAIL refcount\n"); return 1; }
    if (g_released != 0) { std::printf("FAIL released\n"); return 1; }
    inst.flag = 0;
    r = CTCWeapon_IsAnalogueLoading(&w);
    if (r != false) { std::printf("FAIL result2\n"); return 1; }
    std::printf("CTCWeapon_005bd4ce_TEST PASS\n");
    return 0;
}