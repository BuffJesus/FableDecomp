#include <cstdio>

struct CLandscapeBackgroundPatch { int x; };

static int g_dtor_called = 0;
static int g_delete_called = 0;

void __fastcall CLBP_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
void __cdecl CLBP_operator_delete(void* self) { (void)self; g_delete_called++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CLBP_dtor(self);
    if (flags & 1) {
        CLBP_operator_delete(self);
    }
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    g_dtor_called = 0; g_delete_called = 0;
    void* r0 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r0 != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL flags=0\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL flags=1\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_0045709b_TEST PASS\n");
    return 0;
}