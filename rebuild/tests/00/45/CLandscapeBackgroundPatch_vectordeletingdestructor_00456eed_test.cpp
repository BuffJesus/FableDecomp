#include <cstdio>

struct CLandscapeBackgroundPatch { int dummy; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_calls++; }
void __cdecl op_delete(void* p) { (void)p; g_delete_calls++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        op_delete((void*)self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj; obj.dummy = 42;

    // flags without bit 0: destruct only, no delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r0 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r0 != &obj || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL branch0\n"); return 1; }

    // flags with bit 0: destruct + delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r1 != &obj || g_dtor_calls != 1 || g_delete_calls != 1) { std::printf("FAIL branch1\n"); return 1; }

    // even flag value (bit0 clear) => no delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (r2 != &obj || g_delete_calls != 0) { std::printf("FAIL branch2\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00456eed_TEST PASS\n");
    return 0;
}