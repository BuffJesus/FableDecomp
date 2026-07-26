#include <cstdio>

static int g_scalar_called = 0;
static int g_delete_called = 0;

struct CLandscapeBackgroundPatch {
    void* vftable;
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};
extern "C" void __fastcall CLandscapeBackgroundPatch_scalar_dtor(CLandscapeBackgroundPatch* self) { g_scalar_called++; (void)self; }
extern "C" void __cdecl fable_operator_delete(void* p) { g_delete_called++; (void)p; }

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch_scalar_dtor(this);
    if (flags & 1)
        fable_operator_delete((void*)this);
    return (void*)this;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;

    g_scalar_called = 0; g_delete_called = 0;
    void* r1 = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(0);
    if (r1 != &obj || g_scalar_called != 1 || g_delete_called != 0) { std::printf("FAIL flag0\n"); return 1; }

    g_scalar_called = 0; g_delete_called = 0;
    void* r2 = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(1);
    if (r2 != &obj || g_scalar_called != 1 || g_delete_called != 1) { std::printf("FAIL flag1\n"); return 1; }

    g_scalar_called = 0; g_delete_called = 0;
    void* r3 = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(2);
    if (r3 != &obj || g_scalar_called != 1 || g_delete_called != 0) { std::printf("FAIL flag2\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00443080_TEST PASS\n");
    return 0;
}