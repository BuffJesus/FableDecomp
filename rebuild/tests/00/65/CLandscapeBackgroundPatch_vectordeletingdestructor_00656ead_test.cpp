#include <cstdio>

// Standalone behaviour model of the vector deleting destructor.
static int g_destroyed;
static int g_freed;

struct Sub { int v; };

struct Obj {
    char pad[0x10];
    Sub  sub;
};

static void sub_destroy(Sub* s) { s->v = 0; g_destroyed++; }
static void my_delete(void* p) { (void)p; g_freed++; }

static void* vdd(Obj* self, unsigned int flags)
{
    sub_destroy(&self->sub);
    if (flags & 1)
        my_delete(self);
    return self;
}

int main()
{
    Obj a; a.sub.v = 5;
    g_destroyed = g_freed = 0;
    void* r = vdd(&a, 0);
    if (r != &a) { printf("FAIL ret0\n"); return 1; }
    if (g_destroyed != 1) { printf("FAIL destroy0\n"); return 1; }
    if (g_freed != 0) { printf("FAIL free0\n"); return 1; }

    g_destroyed = g_freed = 0;
    r = vdd(&a, 1);
    if (r != &a) { printf("FAIL ret1\n"); return 1; }
    if (g_destroyed != 1) { printf("FAIL destroy1\n"); return 1; }
    if (g_freed != 1) { printf("FAIL free1\n"); return 1; }

    g_freed = 0;
    vdd(&a, 2);
    if (g_freed != 0) { printf("FAIL free2\n"); return 1; }

    printf("VDD_OK\n");
    return 0;
}