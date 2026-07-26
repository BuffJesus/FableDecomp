#include <cstdio>

struct Counted {
    void (__fastcall **vtbl)(Counted*);
    long ref;
};
struct Ptr {
    Counted* p;
};

static int g_dtor_called = 0;
static void __fastcall FakeDtor(Counted* c) { g_dtor_called++; }
static void (__fastcall *g_vtbl[2])(Counted*) = { 0, FakeDtor };

void __fastcall Dest_val(Ptr* self)
{
    Counted* c = self->p;
    if (c != 0) {
        if (--c->ref == 0) {
            c->vtbl[1](c);
        }
        self->p = 0;
    }
}

int main()
{
    /* Case 1: null pointer -> nothing happens, stays null */
    Ptr a; a.p = 0;
    Dest_val(&a);
    if (a.p != 0) { std::printf("FAIL null case\n"); return 1; }

    /* Case 2: refcount > 1 -> just decrement, no dtor, ptr cleared */
    Counted c2; c2.vtbl = g_vtbl; c2.ref = 3;
    Ptr b; b.p = &c2;
    g_dtor_called = 0;
    Dest_val(&b);
    if (b.p != 0) { std::printf("FAIL clear case2\n"); return 1; }
    if (c2.ref != 2) { std::printf("FAIL ref case2\n"); return 1; }
    if (g_dtor_called != 0) { std::printf("FAIL dtor case2\n"); return 1; }

    /* Case 3: refcount == 1 -> decrement to 0, dtor invoked, ptr cleared */
    Counted c3; c3.vtbl = g_vtbl; c3.ref = 1;
    Ptr d; d.p = &c3;
    g_dtor_called = 0;
    Dest_val(&d);
    if (d.p != 0) { std::printf("FAIL clear case3\n"); return 1; }
    if (c3.ref != 0) { std::printf("FAIL ref case3\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor case3\n"); return 1; }

    std::printf("DestVal_00403e20_TEST PASS\n");
    return 0;
}