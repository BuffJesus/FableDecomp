#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_last = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(void* self) { g_dtor_called++; g_last = self; }
void __cdecl op_delete(void* p) { g_delete_called++; (void)p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int edx, unsigned int flags)
{
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main()
{
    int obj = 0;
    void* p = &obj;

    g_dtor_called = g_delete_called = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0, 0);
    if (r != p || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL noflag\n"); return 1; }

    g_dtor_called = g_delete_called = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0, 1);
    if (r != p || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL flag\n"); return 1; }

    g_dtor_called = g_delete_called = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0, 2);
    if (r != p || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL flag2\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_004563ad_TEST PASS\n");
    return 0;
}