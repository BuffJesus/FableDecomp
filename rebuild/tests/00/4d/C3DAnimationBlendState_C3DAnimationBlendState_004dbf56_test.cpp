#include <stdio.h>

struct RefVtbl;
struct RefCounted {
    RefVtbl* vtbl;
    int      refs;
};
struct RefVtbl {
    void (*slot0)(RefCounted*);
    void (*slot1)(RefCounted*);
};

static int g_released = 0;
static RefCounted* g_freed = 0;
static void do_release(RefCounted* p) { g_released++; g_freed = p; }
static RefVtbl g_vtbl = { 0, do_release };

struct Obj {
    char pad[0x14];
    RefCounted* ptr;
};

// Re-implementation of the destructor body under test.
static void dtor_body(Obj* self)
{
    RefCounted* p = self->ptr;
    if (p) {
        if (--p->refs == 0) {
            p->vtbl->slot1(p);
        }
        self->ptr = 0;
    }
}

int main()
{
    // Case 1: null ptr -> nothing happens
    Obj a; a.ptr = 0;
    dtor_body(&a);
    if (g_released != 0 || a.ptr != 0) { printf("FAIL null\n"); return 1; }

    // Case 2: refs>1 -> decrement, no release, ptr cleared
    RefCounted r2; r2.vtbl = &g_vtbl; r2.refs = 2;
    Obj b; b.ptr = &r2;
    dtor_body(&b);
    if (g_released != 0) { printf("FAIL noderef\n"); return 1; }
    if (r2.refs != 1) { printf("FAIL refs\n"); return 1; }
    if (b.ptr != 0) { printf("FAIL clear2\n"); return 1; }

    // Case 3: refs==1 -> decrement to 0, release called, ptr cleared
    RefCounted r3; r3.vtbl = &g_vtbl; r3.refs = 1;
    Obj c; c.ptr = &r3;
    dtor_body(&c);
    if (g_released != 1 || g_freed != &r3) { printf("FAIL release\n"); return 1; }
    if (r3.refs != 0) { printf("FAIL zero\n"); return 1; }
    if (c.ptr != 0) { printf("FAIL clear3\n"); return 1; }

    printf("BLENDSTATE_DTOR_OK\n");
    return 0;
}