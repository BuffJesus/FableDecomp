#include <cstdio>
struct CLandscapeBackgroundPatch { int x; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
static void __cdecl op_delete(void* p) { (void)p; g_delete_called++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        op_delete((void*)self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.x = 7;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj) { std::printf("FAIL ret\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor0\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL del0\n"); return 1; }
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor_called != 2) { std::printf("FAIL dtor1\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL del1\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_004554aa_TEST PASS\n");
    return 0;
}