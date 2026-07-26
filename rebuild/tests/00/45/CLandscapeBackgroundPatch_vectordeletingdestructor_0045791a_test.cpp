#include <cstdio>

struct CLandscapeBackgroundPatch { int marker; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self)
{
    (void)self;
    ++g_dtor_calls;
}
void __cdecl fable_operator_delete(void* p)
{
    (void)p;
    ++g_delete_calls;
}

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags)
{
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        fable_operator_delete(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj; obj.marker = 42;

    g_dtor_calls = 0; g_delete_calls = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj) { std::printf("FAIL: return ptr mismatch flags=0\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL: dtor not called flags=0\n"); return 1; }
    if (g_delete_calls != 0) { std::printf("FAIL: delete called with flags=0\n"); return 1; }

    g_dtor_calls = 0; g_delete_calls = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj) { std::printf("FAIL: return ptr mismatch flags=1\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL: dtor not called flags=1\n"); return 1; }
    if (g_delete_calls != 1) { std::printf("FAIL: delete not called flags=1\n"); return 1; }

    g_dtor_calls = 0; g_delete_calls = 0;
    CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (g_delete_calls != 0) { std::printf("FAIL: delete called when bit0 clear (flags=2)\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_0045791a_TEST PASS\n");
    return 0;
}