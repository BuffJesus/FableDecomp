#include <stdio.h>

struct CAnimComponentBase {
    virtual void v0() {}
    virtual void release() {}
};

struct PairHolder {
    char pad[0x2c];
    CAnimComponentBase* ptr;
};

void __fastcall _Dest_val_release(PairHolder* self, int edx);

static int g_released = 0;

struct Impl : CAnimComponentBase {
    int refcount; // at +4 after vptr
    void v0() {}
    void release() { g_released = 1; }
};

int main()
{
    // Case 1: refcount goes to 0 -> release called, ptr cleared.
    Impl obj;
    // layout: vtable at +0, refcount at +4. Impl has a vptr at +0 already.
    int* rc = (int*)((char*)&obj + 4);
    *rc = 1;
    PairHolder h;
    h.ptr = &obj;
    g_released = 0;
    _Dest_val_release(&h, 0);
    if (g_released != 1) { printf("FAIL release\n"); return 1; }
    if (h.ptr != 0) { printf("FAIL not cleared\n"); return 1; }

    // Case 2: refcount > 1 -> no release, still cleared.
    Impl obj2;
    int* rc2 = (int*)((char*)&obj2 + 4);
    *rc2 = 2;
    PairHolder h2;
    h2.ptr = &obj2;
    g_released = 0;
    _Dest_val_release(&h2, 0);
    if (g_released != 0) { printf("FAIL early release\n"); return 1; }
    if (*rc2 != 1) { printf("FAIL rc not dec\n"); return 1; }
    if (h2.ptr != 0) { printf("FAIL h2 not cleared\n"); return 1; }

    // Case 3: null ptr -> nothing happens.
    PairHolder h3;
    h3.ptr = 0;
    g_released = 0;
    _Dest_val_release(&h3, 0);
    if (g_released != 0) { printf("FAIL null release\n"); return 1; }

    printf("DESTVAL_OK\n");
    return 0;
}