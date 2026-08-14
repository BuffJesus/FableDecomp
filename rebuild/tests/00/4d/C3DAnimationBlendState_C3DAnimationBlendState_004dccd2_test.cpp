#include <cstdio>

struct RefCounted;
struct RefCountedVtbl {
    void (*dtor)(RefCounted*);
    void (*release)(RefCounted*);
};
struct RefCounted {
    RefCountedVtbl* vtbl;
    int refcount;
};

static int g_released = 0;
static void do_release(RefCounted* p) { g_released++; }
static RefCountedVtbl g_vtbl = { 0, do_release };

struct Owner {
    char pad[0xc];
    RefCounted* ptr;
};

// re-implement the destructor body (member release + zero)
static void release_member(Owner* self) {
    RefCounted* p = self->ptr;
    if (p) {
        if (--p->refcount == 0) {
            p->vtbl->release(p);
        }
    }
    self->ptr = 0;
}

int main() {
    // case 1: null pointer -> nothing happens
    Owner a; a.ptr = 0;
    release_member(&a);
    if (a.ptr != 0) { printf("FAIL null\n"); return 1; }
    if (g_released != 0) { printf("FAIL null2\n"); return 1; }

    // case 2: refcount 2 -> decremented to 1, no release, ptr zeroed
    RefCounted r2; r2.vtbl = &g_vtbl; r2.refcount = 2;
    Owner b; b.ptr = &r2;
    release_member(&b);
    if (r2.refcount != 1) { printf("FAIL rc2\n"); return 1; }
    if (g_released != 0) { printf("FAIL rel2\n"); return 1; }
    if (b.ptr != 0) { printf("FAIL zero2\n"); return 1; }

    // case 3: refcount 1 -> hits 0, release called, ptr zeroed
    RefCounted r1; r1.vtbl = &g_vtbl; r1.refcount = 1;
    Owner c; c.ptr = &r1;
    release_member(&c);
    if (r1.refcount != 0) { printf("FAIL rc1\n"); return 1; }
    if (g_released != 1) { printf("FAIL rel1\n"); return 1; }
    if (c.ptr != 0) { printf("FAIL zero1\n"); return 1; }

    printf("BLENDSTATE_DTOR_OK\n");
    return 0;
}