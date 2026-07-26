#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;

static void CLandscapeBackgroundPatch_dtor(void* self) { (void)self; g_dtor_called++; }
static void op_delete(void* p) { (void)p; g_delete_called++; }

static void* CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main()
{
    int obj = 0;
    void* p = &obj;

    g_dtor_called = 0; g_delete_called = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0);
    if (r != p || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL noflag\n"); return 1;
    }

    g_dtor_called = 0; g_delete_called = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 1);
    if (r != p || g_dtor_called != 1 || g_delete_called != 1) {
        std::printf("FAIL flag\n"); return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00456851_TEST PASS\n");
    return 0;
}