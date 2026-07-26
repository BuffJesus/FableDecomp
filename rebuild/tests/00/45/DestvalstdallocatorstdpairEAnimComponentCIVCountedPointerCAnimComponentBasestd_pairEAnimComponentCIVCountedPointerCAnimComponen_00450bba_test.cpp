#include <cstdio>

struct CtrlBlock {
    void** vtbl;
    int    refcount;
};
struct Pair {
    int          first;
    int          pad;
    void*        ptr;
    CtrlBlock*   held;
};

static int g_dtor_called = 0;
static void __fastcall fake_dtor(CtrlBlock* c) { (void)c; g_dtor_called = 1; }

// Semantic model equivalent to the naked-asm body's primary path.
void __fastcall Dest_val_model(Pair* self)
{
    CtrlBlock* c = self->held;
    if (c) {
        if (--c->refcount == 0) {
            ((void(__fastcall*)(CtrlBlock*))(c->vtbl[1]))(c);
        }
        self->held = 0;
    }
}

int main() {
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&fake_dtor;

    CtrlBlock c;  c.vtbl = (void**)vt; c.refcount = 1;
    Pair p; p.first = 3; p.pad = 0; p.ptr = 0; p.held = &c;
    Dest_val_model(&p);
    if (g_dtor_called != 1) { std::printf("FAIL dtor not called\n"); return 1; }
    if (p.held != 0)        { std::printf("FAIL held not nulled\n"); return 1; }

    g_dtor_called = 0;
    CtrlBlock c2; c2.vtbl = (void**)vt; c2.refcount = 2;
    Pair p2; p2.first=0; p2.pad=0; p2.ptr=0; p2.held=&c2;
    Dest_val_model(&p2);
    if (g_dtor_called != 0) { std::printf("FAIL dtor called early\n"); return 1; }
    if (c2.refcount != 1)   { std::printf("FAIL refcount\n"); return 1; }
    if (p2.held != 0)       { std::printf("FAIL held2 not nulled\n"); return 1; }

    Pair p3; p3.first=0; p3.pad=0; p3.ptr=0; p3.held=0;
    Dest_val_model(&p3);

    std::printf("Destval_00450bba_TEST PASS\n");
    return 0;
}