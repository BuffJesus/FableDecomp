#include <cstdio>

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_last_deleted = 0;

static void CLandscapeBackgroundPatch_dtor(void* self) { (void)self; ++g_dtor_calls; }
static void operator_delete_impl(void* p) { ++g_delete_calls; g_last_deleted = p; }

static void* CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete_impl(self);
    return self;
}

int main()
{
    int obj = 42;
    void* p = &obj;

    // flag bit clear: dtor runs, no delete
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0);
    if (r != p || g_dtor_calls != 1 || g_delete_calls != 0) {
        std::printf("FAIL noflag dtor=%d del=%d\n", g_dtor_calls, g_delete_calls);
        return 1;
    }

    // flag bit set: dtor + delete
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 1);
    if (r != p || g_dtor_calls != 1 || g_delete_calls != 1 || g_last_deleted != p) {
        std::printf("FAIL flag dtor=%d del=%d\n", g_dtor_calls, g_delete_calls);
        return 1;
    }

    // even flag (bit0 clear) must not delete
    g_dtor_calls = g_delete_calls = 0;
    CLandscapeBackgroundPatch_vector_deleting_destructor(p, 2);
    if (g_delete_calls != 0) { std::printf("FAIL evenflag\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00450be3_TEST PASS\n");
    return 0;
}