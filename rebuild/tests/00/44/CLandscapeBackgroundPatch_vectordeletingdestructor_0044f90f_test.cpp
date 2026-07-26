#include <cstdio>

struct CLandscapeBackgroundPatch {
    void* vptr;
};

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_vftable_marker = (void*)0x1230ba0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { g_dtor_called++; (void)self; }
void __cdecl op_delete(void* p) { g_delete_called++; (void)p; }
void* CLandscapeBackgroundPatch_vftable = 0;

void* __fastcall CLandscapeBackgroundPatch_scalar_deleting_destructor(CLandscapeBackgroundPatch* self, int flags)
{
    self->vptr = &CLandscapeBackgroundPatch_vftable;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        op_delete(self);
    }
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.vptr = 0;

    // flags bit0 clear: dtor runs, no delete
    g_dtor_called = 0; g_delete_called = 0;
    void* r = CLandscapeBackgroundPatch_scalar_deleting_destructor(&obj, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL path0 r=%p dtor=%d del=%d\n", r, g_dtor_called, g_delete_called);
        return 1;
    }
    if (obj.vptr != &CLandscapeBackgroundPatch_vftable) {
        std::printf("FAIL vptr not set\n");
        return 1;
    }

    // flags bit0 set: dtor + delete
    g_dtor_called = 0; g_delete_called = 0;
    r = CLandscapeBackgroundPatch_scalar_deleting_destructor(&obj, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) {
        std::printf("FAIL path1 r=%p dtor=%d del=%d\n", r, g_dtor_called, g_delete_called);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0044f90f_TEST PASS\n");
    return 0;
}