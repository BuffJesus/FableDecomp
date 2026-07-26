#include <cstdio>
struct CLandscapeBackgroundPatch { void* vftable; int dead; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self){ g_dtor_called++; self->dead = 1; }
void __cdecl op_delete(void* p){ (void)p; g_delete_called++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_ignored, unsigned int flags)
{
    (void)edx_ignored;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main(){
    CLandscapeBackgroundPatch obj; obj.vftable=0; obj.dead=0;
    g_dtor_called=0; g_delete_called=0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL noflag\n"); return 1; }
    g_dtor_called=0; g_delete_called=0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL flag\n"); return 1; }
    g_dtor_called=0; g_delete_called=0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL flag2\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00442da0_TEST PASS\n");
    return 0;
}