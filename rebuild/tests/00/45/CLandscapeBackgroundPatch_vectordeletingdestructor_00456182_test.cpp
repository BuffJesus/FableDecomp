#include <cstdio>
#include <cstdlib>
struct CLandscapeBackgroundPatch { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_last_deleted = 0;
static void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }

static void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_ignored, unsigned int flags)
{
    (void)edx_ignored;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) { g_delete_called++; g_last_deleted = (void*)self; }
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj; obj.f0 = 42;
    g_dtor_called = 0; g_delete_called = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL noflag\n"); return 1; }
    g_dtor_called = 0; g_delete_called = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1 || g_last_deleted != &obj) { std::printf("FAIL flag\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00456182_TEST PASS\n");
    return 0;
}