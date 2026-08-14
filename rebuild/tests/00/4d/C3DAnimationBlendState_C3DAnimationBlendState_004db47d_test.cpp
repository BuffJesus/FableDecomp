#include <cstdio>

struct RefBlock {
    void** vtbl;
    int refcount;
};

struct C3DAnimationBlendState {
    char pad[0x10];
    RefBlock* ptr; // +0x10
};

static int g_base_dtor_called = 0;
static int g_virtual_called = 0;

void __fastcall base_dtor(C3DAnimationBlendState* self) {
    (void)self;
    g_base_dtor_called++;
}

void __fastcall vfn(RefBlock* p) {
    (void)p;
    g_virtual_called++;
}

void __fastcall dtor(C3DAnimationBlendState* self)
{
    RefBlock* p = self->ptr;
    if (p) {
        if (--p->refcount == 0) {
            ((void(__fastcall*)(RefBlock*))p->vtbl[1])(p);
        }
        self->ptr = 0;
    }
    base_dtor(self);
}

int main() {
    void* vt[2] = { 0, (void*)&vfn };

    // Case A: ptr null -> just base dtor
    C3DAnimationBlendState a;
    a.ptr = 0;
    g_base_dtor_called = 0; g_virtual_called = 0;
    dtor(&a);
    if (g_base_dtor_called != 1 || g_virtual_called != 0) { printf("BAD_A\n"); return 1; }

    // Case B: refcount 2 -> decrement to 1, no virtual, cleared, base dtor
    RefBlock rb;
    rb.vtbl = (void**)vt;
    rb.refcount = 2;
    C3DAnimationBlendState b;
    b.ptr = &rb;
    g_base_dtor_called = 0; g_virtual_called = 0;
    dtor(&b);
    if (rb.refcount != 1 || g_virtual_called != 0 || b.ptr != 0 || g_base_dtor_called != 1) { printf("BAD_B\n"); return 1; }

    // Case C: refcount 1 -> decrement to 0, virtual called, cleared, base dtor
    rb.refcount = 1;
    C3DAnimationBlendState c;
    c.ptr = &rb;
    g_base_dtor_called = 0; g_virtual_called = 0;
    dtor(&c);
    if (rb.refcount != 0 || g_virtual_called != 1 || c.ptr != 0 || g_base_dtor_called != 1) { printf("BAD_C\n"); return 1; }

    printf("BLENDSTATE_DTOR_OK\n");
    return 0;
}