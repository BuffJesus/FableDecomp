#include <cstdio>

// Independent behaviour test for the vector deleting destructor shape.

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void *g_last_deleted = 0;

struct CLandscapeBackgroundPatch
{
    int marker;
    void * vecdtor(unsigned int flags);
};

static void scalar_dtor(void *self)
{
    (void)self;
    ++g_dtor_calls;
}

static void op_delete(void *p)
{
    ++g_delete_calls;
    g_last_deleted = p;
}

void * CLandscapeBackgroundPatch::vecdtor(unsigned int flags)
{
    scalar_dtor(this);
    if (flags & 1)
        op_delete(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch a;
    a.marker = 0x1234;

    // flags bit0 clear: dtor runs, no delete, returns this
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void *r0 = a.vecdtor(0);
    if (r0 != &a || g_dtor_calls != 1 || g_delete_calls != 0)
    {
        printf("FAIL case0\n");
        return 1;
    }

    // flags bit0 set: dtor runs, delete runs on this, returns this
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void *r1 = a.vecdtor(1);
    if (r1 != &a || g_dtor_calls != 1 || g_delete_calls != 1 || g_last_deleted != &a)
    {
        printf("FAIL case1\n");
        return 1;
    }

    // even flags (bit0 clear) must not delete
    g_dtor_calls = g_delete_calls = 0;
    a.vecdtor(2);
    if (g_delete_calls != 0)
    {
        printf("FAIL case2\n");
        return 1;
    }

    printf("VECDTOR_OK\n");
    return 0;
}