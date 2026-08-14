#include <stdio.h>

struct Obj { int alive; };

static int g_dtor = 0;
static int g_freed = 0;

static void model_dtor(Obj* self) { self->alive = 0; g_dtor++; }
static void model_dealloc(Obj* self, int edx) { (void)self; (void)edx; g_freed++; }

static void* model_vdtor(Obj* self, unsigned int flags)
{
    model_dtor(self);
    if (flags & 1) {
        model_dealloc(self, 0);
    }
    return self;
}

int main()
{
    Obj a; a.alive = 1;
    // flags bit0 clear: dtor only, no free
    void* r1 = model_vdtor(&a, 0);
    if (r1 != &a) { printf("FAIL ret1\n"); return 1; }
    if (g_dtor != 1 || g_freed != 0 || a.alive != 0) { printf("FAIL nofree\n"); return 1; }

    Obj b; b.alive = 1;
    // flags bit0 set: dtor + free
    void* r2 = model_vdtor(&b, 1);
    if (r2 != &b) { printf("FAIL ret2\n"); return 1; }
    if (g_dtor != 2 || g_freed != 1) { printf("FAIL free\n"); return 1; }

    // even flags (bit0 clear) must not free
    Obj c; c.alive = 1;
    model_vdtor(&c, 2);
    if (g_freed != 1) { printf("FAIL evenflag\n"); return 1; }

    printf("VDTOR_OK\n");
    return 0;
}