#include <cstdio>
struct CLandscapeBackgroundPatch { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
static void __cdecl eng_operator_delete(void* p) { (void)p; g_delete_called++; }

static void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        eng_operator_delete(self);
    return self;
}

int main() {
    CLandscapeBackgroundPatch obj; obj.f0 = 5;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL flags0\n"); return 1; }
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_called != 2 || g_delete_called != 1) { std::printf("FAIL flags1\n"); return 1; }
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (r != &obj || g_delete_called != 1) { std::printf("FAIL flags2\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00455ff4_TEST PASS\n");
    return 0;
}