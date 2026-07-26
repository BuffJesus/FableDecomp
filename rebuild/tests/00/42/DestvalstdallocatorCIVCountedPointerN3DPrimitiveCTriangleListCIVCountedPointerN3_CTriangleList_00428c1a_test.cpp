#include <cstdio>

static int g_released = 0;

struct CObj {
    virtual void slot0() {}
    virtual void Release() { g_released++; }
    int refcount;
};
struct Holder {
    CObj* p;
};

void __fastcall _Dest_val(Holder* self)
{
    CObj* p = self->p;
    if (p) {
        if (--p->refcount == 0) {
            p->Release();
        }
        self->p = 0;
    }
}

int main()
{
    // Case 1: refcount 1 -> drops to 0 -> Release called, pointer nulled
    CObj a; a.refcount = 1;
    Holder h; h.p = &a;
    _Dest_val(&h);
    if (g_released != 1 || h.p != 0) {
        std::printf("FAIL case1 rel=%d p=%p\n", g_released, (void*)h.p);
        return 1;
    }

    // Case 2: refcount 2 -> drops to 1 -> no Release, still nulled
    CObj b; b.refcount = 2;
    Holder h2; h2.p = &b;
    _Dest_val(&h2);
    if (g_released != 1 || b.refcount != 1 || h2.p != 0) {
        std::printf("FAIL case2 rel=%d rc=%d p=%p\n", g_released, b.refcount, (void*)h2.p);
        return 1;
    }

    // Case 3: null pointer -> nothing
    Holder h3; h3.p = 0;
    _Dest_val(&h3);
    if (g_released != 1 || h3.p != 0) {
        std::printf("FAIL case3 rel=%d\n", g_released);
        return 1;
    }

    std::printf("_Dest_val_00428c1a_TEST PASS\n");
    return 0;
}