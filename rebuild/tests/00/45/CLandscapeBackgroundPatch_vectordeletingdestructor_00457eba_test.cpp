#include <cstdio>

struct CLandscapeBackgroundPatch { void* vftable; };

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_deleted = 0;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
extern "C" void __cdecl eng_operator_delete(void* p) { g_delete_called++; g_deleted = p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags)
{
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        eng_operator_delete(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;

    g_dtor_called = 0; g_delete_called = 0; g_deleted = 0;
    void* r0 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r0 != &obj || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL flags=0 r=%p dtor=%d del=%d\n", r0, g_dtor_called, g_delete_called);
        return 1;
    }

    g_dtor_called = 0; g_delete_called = 0; g_deleted = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 1 || g_deleted != &obj) {
        std::printf("FAIL flags=1 r=%p dtor=%d del=%d\n", r1, g_dtor_called, g_delete_called);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00457eba_TEST PASS\n");
    return 0;
}