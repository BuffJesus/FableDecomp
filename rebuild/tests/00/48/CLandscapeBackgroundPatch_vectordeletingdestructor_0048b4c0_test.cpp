#include <cstdio>
#include <cstdlib>

struct CLandscapeBackgroundPatch;

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_last_deleted = 0;

void __fastcall CLandscapeBackgroundPatch_scalar_dtor(CLandscapeBackgroundPatch* self)
{
    (void)self;
    g_dtor_calls++;
}

void operator_delete_impl(void* p)
{
    g_delete_calls++;
    g_last_deleted = p;
}

struct CLandscapeBackgroundPatch
{
    int payload;
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch_scalar_dtor(this);
    if (flags & 1)
        operator_delete_impl(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch a;
    a.payload = 42;

    // flags & 1 == 0: dtor runs, no delete, returns this
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void* r0 = a.vector_deleting_destructor(0);
    bool ok0 = (r0 == &a) && (g_dtor_calls == 1) && (g_delete_calls == 0);

    // flags & 1 == 1: dtor runs, delete runs on this, returns this
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void* r1 = a.vector_deleting_destructor(1);
    bool ok1 = (r1 == &a) && (g_dtor_calls == 1) && (g_delete_calls == 1) && (g_last_deleted == &a);

    // even flag (2) -> low bit clear -> no delete
    g_dtor_calls = g_delete_calls = 0;
    void* r2 = a.vector_deleting_destructor(2);
    bool ok2 = (r2 == &a) && (g_dtor_calls == 1) && (g_delete_calls == 0);

    if (ok0 && ok1 && ok2) {
        printf("VECDELDTOR_OK\n");
        return 0;
    }
    printf("FAIL o0=%d o1=%d o2=%d\n", ok0, ok1, ok2);
    return 1;
}