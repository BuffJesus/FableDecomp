// Behaviour model for the scalar/vector deleting destructor.
#include <cstdio>

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_last_deleted = 0;

struct Obj { int x; };

static void inner_dtor(Obj* self) { (void)self; ++g_dtor_calls; }
static void op_delete(void* p) { ++g_delete_calls; g_last_deleted = p; }

static void* vdtor(Obj* self, unsigned int flags)
{
    inner_dtor(self);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main()
{
    Obj a;

    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void* r0 = vdtor(&a, 0);
    bool ok0 = (g_dtor_calls == 1) && (g_delete_calls == 0) && (r0 == &a);

    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void* r1 = vdtor(&a, 1);
    bool ok1 = (g_dtor_calls == 1) && (g_delete_calls == 1) && (g_last_deleted == &a) && (r1 == &a);

    g_dtor_calls = g_delete_calls = 0;
    void* r2 = vdtor(&a, 2);
    bool ok2 = (g_dtor_calls == 1) && (g_delete_calls == 0) && (r2 == &a);

    g_dtor_calls = g_delete_calls = 0;
    vdtor(&a, 3);
    bool ok3 = (g_delete_calls == 1);

    if (ok0 && ok1 && ok2 && ok3)
        printf("VDTOR_OK\n");
    else
        printf("VDTOR_FAIL %d %d %d %d\n", ok0, ok1, ok2, ok3);
    return 0;
}