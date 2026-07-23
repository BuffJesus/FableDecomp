#include <cstdio>
struct CLandscapeBackgroundPatch { int x; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { g_dtor_called++; (void)self; }
extern "C" void operator_delete_stub(void* p) { g_delete_called++; (void)p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        operator_delete_stub((void*)self);
    }
    return (void*)self;
}

int main() {
    CLandscapeBackgroundPatch obj; obj.x = 7;
    g_dtor_called = 0; g_delete_called = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj) { std::printf("FAIL ret1\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor1\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL del1\n"); return 1; }
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor_called != 2) { std::printf("FAIL dtor2\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL del2\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_004133a0_TEST PASS\n");
    return 0;
}