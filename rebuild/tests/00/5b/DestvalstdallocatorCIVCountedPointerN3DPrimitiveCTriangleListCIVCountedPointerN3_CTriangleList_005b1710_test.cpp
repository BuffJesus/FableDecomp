#include <cstdio>

struct Counted {
    void** vtbl;
    int    refCount;
};
struct Ptr { Counted* p; };

static int g_destroyed = 0;

void __fastcall destroyFn(Counted* c) { (void)c; g_destroyed = 1; }

void __fastcall rel(Ptr* self)
{
    Counted* c = self->p;
    if (c != 0) {
        if (--c->refCount == 0) {
            typedef void (__fastcall *Fn)(Counted*);
            ((Fn)(c->vtbl[1]))(c);
        }
        self->p = 0;
    }
}

int main()
{
    void* vt[2] = { 0, (void*)&destroyFn };

    // Case 1: null pointer -> no-op
    Ptr a; a.p = 0;
    rel(&a);
    if (a.p != 0) { printf("FAIL null\n"); return 1; }

    // Case 2: refcount 2 -> decrement to 1, no destroy, ptr nulled
    Counted c2; c2.vtbl = vt; c2.refCount = 2;
    Ptr b; b.p = &c2;
    g_destroyed = 0;
    rel(&b);
    if (c2.refCount != 1) { printf("FAIL dec\n"); return 1; }
    if (g_destroyed != 0) { printf("FAIL nodestroy\n"); return 1; }
    if (b.p != 0) { printf("FAIL null2\n"); return 1; }

    // Case 3: refcount 1 -> decrement to 0, destroy invoked, ptr nulled
    Counted c3; c3.vtbl = vt; c3.refCount = 1;
    Ptr d; d.p = &c3;
    g_destroyed = 0;
    rel(&d);
    if (c3.refCount != 0) { printf("FAIL dec0\n"); return 1; }
    if (g_destroyed != 1) { printf("FAIL destroy\n"); return 1; }
    if (d.p != 0) { printf("FAIL null3\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}