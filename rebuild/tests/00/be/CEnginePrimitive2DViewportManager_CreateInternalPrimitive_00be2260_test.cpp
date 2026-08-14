// Behaviour model for CreateInternalPrimitive: alloc, run ctor, return.
#include <stdio.h>
#include <stdlib.h>

static int g_ctorCalls = 0;

struct Prim { int tag; };

static Prim* ctor(Prim* self) {
    self->tag = 0x1234;
    ++g_ctorCalls;
    return self;
}

static Prim* CreateInternalPrimitive(void* self, int type) {
    (void)self; (void)type;
    void* p = malloc(0xA0);
    if (p != 0)
        return ctor((Prim*)p);
    return 0;
}

int main() {
    Prim* r = CreateInternalPrimitive((void*)1, 7);
    int ok = (r != 0) && (r->tag == 0x1234) && (g_ctorCalls == 1);
    free(r);
    if (ok)
        printf("PRIM_FACTORY_OK\n");
    else
        printf("FAIL\n");
    return 0;
}