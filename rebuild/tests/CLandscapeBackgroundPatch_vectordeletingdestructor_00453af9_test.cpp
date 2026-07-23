#include <cstdio>
struct CLandscapeBackgroundPatch { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
static void __cdecl op_delete(void* p) { (void)p; g_delete_called++; }

struct CLandscapeBackgroundPatch_shim {
    void* CLandscapeBackgroundPatch_vecdel(unsigned int flags);
};

void* CLandscapeBackgroundPatch_shim::CLandscapeBackgroundPatch_vecdel(unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor((CLandscapeBackgroundPatch*)this);
    if (flags & 1)
        op_delete(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch_shim obj;
    g_dtor_called = 0; g_delete_called = 0;
    void* r1 = obj.CLandscapeBackgroundPatch_vecdel(0);
    if (r1 != &obj) { std::printf("FAIL ret0\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor0\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL del0\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0;
    void* r2 = obj.CLandscapeBackgroundPatch_vecdel(1);
    if (r2 != &obj) { std::printf("FAIL ret1\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor1\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL del1\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00453af9_TEST PASS\n");
    return 0;
}