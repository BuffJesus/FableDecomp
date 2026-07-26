#include <cstdio>
struct CLandscapeBackgroundPatch { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self){ g_dtor_called++; (void)self; }
void __cdecl engine_operator_delete(void* p){ g_delete_called++; (void)p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        engine_operator_delete(self);
    return self;
}

int main(){
    CLandscapeBackgroundPatch obj; obj.f0 = 7;
    g_dtor_called = 0; g_delete_called = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL ret\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL nodelete\n"); return 1; }
    g_dtor_called = 0; g_delete_called = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor2\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL delete\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00455d28_TEST PASS\n");
    return 0;
}