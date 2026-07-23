#include <cstdio>

struct CCounted {
    void (__fastcall **vtbl)(CCounted*);
    long refcount;
};

struct CIVCountedPointer {
    CCounted* ptr;
};

void __fastcall Dest_val(CIVCountedPointer* self)
{
    CCounted* c = self->ptr;
    if (c) {
        if (--c->refcount == 0) {
            c->vtbl[1](c);
        }
        self->ptr = 0;
    }
}

static int g_deleted = 0;
static void __fastcall fake_dtor(CCounted* c) { g_deleted = 1; }
static void (__fastcall *g_vt[2])(CCounted*) = { 0, fake_dtor };

int main()
{
    // Case 1: null pointer -> nothing happens
    CIVCountedPointer p1;
    p1.ptr = 0;
    Dest_val(&p1);
    if (p1.ptr != 0) { std::printf("FAIL null\n"); return 1; }

    // Case 2: refcount > 1 -> just decrement, no dtor, clear
    CCounted c2;
    c2.vtbl = g_vt;
    c2.refcount = 3;
    CIVCountedPointer p2;
    p2.ptr = &c2;
    g_deleted = 0;
    Dest_val(&p2);
    if (c2.refcount != 2) { std::printf("FAIL dec\n"); return 1; }
    if (g_deleted != 0) { std::printf("FAIL nodtor\n"); return 1; }
    if (p2.ptr != 0) { std::printf("FAIL clear2\n"); return 1; }

    // Case 3: refcount == 1 -> decrement to 0, call dtor, clear
    CCounted c3;
    c3.vtbl = g_vt;
    c3.refcount = 1;
    CIVCountedPointer p3;
    p3.ptr = &c3;
    g_deleted = 0;
    Dest_val(&p3);
    if (c3.refcount != 0) { std::printf("FAIL dec1\n"); return 1; }
    if (g_deleted != 1) { std::printf("FAIL dtor\n"); return 1; }
    if (p3.ptr != 0) { std::printf("FAIL clear3\n"); return 1; }

    std::printf("Dest_val_0044c180_TEST PASS\n");
    return 0;
}