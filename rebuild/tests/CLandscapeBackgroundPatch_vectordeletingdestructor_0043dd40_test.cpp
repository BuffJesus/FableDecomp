#include <cstdio>

struct CLandscapeBackgroundPatch { void* vftable; };

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_delete_arg = 0;

extern "C" void __fastcall CLandscapeBackgroundPatch_scalar_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
extern "C" void __cdecl CLandscapeBackgroundPatch_op_delete(void* p) { g_delete_called++; g_delete_arg = p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CLandscapeBackgroundPatch_scalar_dtor(self);
    if (flags & 1)
        CLandscapeBackgroundPatch_op_delete(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;

    g_dtor_called = 0; g_delete_called = 0; g_delete_arg = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL ret0\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor0\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL del0\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0; g_delete_arg = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj) { std::printf("FAIL ret1\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor1\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL del1\n"); return 1; }
    if (g_delete_arg != &obj) { std::printf("FAIL delarg\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_0043dd40_TEST PASS\n");
    return 0;
}