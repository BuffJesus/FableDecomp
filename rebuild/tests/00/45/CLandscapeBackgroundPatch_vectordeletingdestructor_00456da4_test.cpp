#include <cstdio>

struct CLandscapeBackgroundPatch { int f0; };

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_deleted_ptr = 0;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
extern "C" void operator_delete_impl(void* p) { g_delete_called++; g_deleted_ptr = p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete_impl(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj; obj.f0 = 7;
    g_dtor_called = 0; g_delete_called = 0; g_deleted_ptr = 0;
    void* r0 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r0 != &obj) { std::printf("FAIL ret0\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor0\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL nodelete\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0; g_deleted_ptr = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r1 != &obj) { std::printf("FAIL ret1\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor1\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL delete1\n"); return 1; }
    if (g_deleted_ptr != &obj) { std::printf("FAIL delptr\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00456da4_TEST PASS\n");
    return 0;
}