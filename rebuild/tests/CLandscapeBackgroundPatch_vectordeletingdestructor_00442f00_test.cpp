#include <cstdio>
struct CLandscapeBackgroundPatch { void* vftable; int destroyed; };
static int g_deleted = 0;
static void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self){ self->destroyed = 1; }
static void __cdecl operator_delete_stub(void* p){ g_deleted = 1; (void)p; }

static void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_pad, unsigned int flags)
{
    (void)edx_pad;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete_stub(self);
    return self;
}

int main(){
    CLandscapeBackgroundPatch o; o.destroyed = 0;
    g_deleted = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&o, 0, 0);
    if (r != &o || o.destroyed != 1 || g_deleted != 0){ std::printf("FAIL flags0\n"); return 1; }
    o.destroyed = 0; g_deleted = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&o, 0, 1);
    if (r != &o || o.destroyed != 1 || g_deleted != 1){ std::printf("FAIL flags1\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00442f00_TEST PASS\n");
    return 0;
}