#include <cstdio>

struct CLandscapeBackgroundPatch { int x; };
static int g_dtor_calls = 0;
static int g_delete_calls = 0;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_calls++; }
extern "C" void __cdecl op_delete(void* p) { (void)p; g_delete_calls++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        op_delete((void*)self);
    return (void*)self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.x = 42;
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL: return mismatch\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL: dtor not called\n"); return 1; }
    if (g_delete_calls != 0) { std::printf("FAIL: delete called when flag=0\n"); return 1; }

    g_dtor_calls = 0; g_delete_calls = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj) { std::printf("FAIL: return mismatch2\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL: dtor not called2\n"); return 1; }
    if (g_delete_calls != 1) { std::printf("FAIL: delete not called when flag=1\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_004582ba_TEST PASS\n");
    return 0;
}