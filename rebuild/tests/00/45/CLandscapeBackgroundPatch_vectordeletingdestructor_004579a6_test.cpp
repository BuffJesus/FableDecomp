#include <cstdio>
struct CLandscapeBackgroundPatch { void* vftable; };
static int g_dtor = 0;
static int g_del = 0;
static void* g_deleted = 0;
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self){ g_dtor++; }
extern "C" void __cdecl fable_operator_delete(void* p){ g_del++; g_deleted = p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        fable_operator_delete(self);
    return self;
}

int main(){
    CLandscapeBackgroundPatch obj; obj.vftable = 0;
    g_dtor = g_del = 0; g_deleted = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor != 1 || g_del != 0){ std::printf("FAIL noflag\n"); return 1; }
    g_dtor = g_del = 0; g_deleted = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor != 1 || g_del != 1 || g_deleted != &obj){ std::printf("FAIL flag\n"); return 1; }
    g_dtor = g_del = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (r != &obj || g_dtor != 1 || g_del != 0){ std::printf("FAIL flag2\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_004579a6_TEST PASS\n");
    return 0;
}