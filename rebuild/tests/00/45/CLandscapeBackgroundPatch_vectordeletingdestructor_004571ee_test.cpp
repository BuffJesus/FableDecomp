#include <cstdio>
struct CLandscapeBackgroundPatch { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
void __cdecl operator_delete(void* p) { (void)p; g_delete_called++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_ignored, unsigned int flags)
{
    (void)edx_ignored;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.f0 = 42;
    g_dtor_called = 0; g_delete_called = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL branch0\n"); return 1; }
    g_dtor_called = 0; g_delete_called = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL branch1\n"); return 1; }
    g_dtor_called = 0; g_delete_called = 0;
    void* r3 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (r3 != &obj || g_delete_called != 0) { std::printf("FAIL branch2\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_004571ee_TEST PASS\n");
    return 0;
}