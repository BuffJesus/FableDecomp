#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(void* self)
{
    (void)self;
    g_dtor_called++;
}
extern "C" void __cdecl fable_operator_delete(void* p)
{
    (void)p;
    g_delete_called++;
}

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int edx, unsigned int flags)
{
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        fable_operator_delete(self);
    return self;
}

int main()
{
    int obj = 0;
    void* p = &obj;

    g_dtor_called = 0; g_delete_called = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0, 0);
    if (r != p || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL: flags=0 path dtor=%d del=%d r=%p\n", g_dtor_called, g_delete_called, r);
        return 1;
    }

    g_dtor_called = 0; g_delete_called = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0, 1);
    if (r != p || g_dtor_called != 1 || g_delete_called != 1) {
        std::printf("FAIL: flags=1 path dtor=%d del=%d r=%p\n", g_dtor_called, g_delete_called, r);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0044fa51_TEST PASS\n");
    return 0;
}