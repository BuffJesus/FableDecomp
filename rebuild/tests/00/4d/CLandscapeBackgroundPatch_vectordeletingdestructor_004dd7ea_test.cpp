// Independent behaviour model for the vector deleting destructor.
#include <cstdio>

typedef unsigned int uint;

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

struct Obj { int x; };

static void scalar_dtor(Obj* self) { (void)self; g_dtor_calls++; }
static void op_delete(void* p) { (void)p; g_delete_calls++; }

static void* vdd(Obj* self, uint flags)
{
    scalar_dtor(self);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main()
{
    Obj a; a.x = 5;

    // flags & 1 == 0 -> dtor runs, no delete
    g_dtor_calls = g_delete_calls = 0;
    void* r0 = vdd(&a, 0);
    bool ok0 = (g_dtor_calls == 1) && (g_delete_calls == 0) && (r0 == &a);

    // flags & 1 == 1 -> dtor runs AND delete
    g_dtor_calls = g_delete_calls = 0;
    void* r1 = vdd(&a, 1);
    bool ok1 = (g_dtor_calls == 1) && (g_delete_calls == 1) && (r1 == &a);

    // flags == 2 (bit0 clear) -> no delete
    g_dtor_calls = g_delete_calls = 0;
    void* r2 = vdd(&a, 2);
    bool ok2 = (g_dtor_calls == 1) && (g_delete_calls == 0) && (r2 == &a);

    // flags == 3 (bit0 set) -> delete
    g_dtor_calls = g_delete_calls = 0;
    void* r3 = vdd(&a, 3);
    bool ok3 = (g_dtor_calls == 1) && (g_delete_calls == 1) && (r3 == &a);

    if (ok0 && ok1 && ok2 && ok3)
        printf("VDD_OK\n");
    else
        printf("VDD_FAIL %d %d %d %d\n", ok0, ok1, ok2, ok3);
    return 0;
}