#include <cstdio>
struct CLandscapeBackgroundPatch { int f0; void* vf; };
static int g_dtor_calls = 0;
static int g_del_calls = 0;
static void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self){ (void)self; ++g_dtor_calls; }
static void operator_delete_stub(void* p){ (void)p; ++g_del_calls; }
static void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete_stub(self);
    return self;
}
int main(){
    CLandscapeBackgroundPatch obj; obj.f0 = 7; obj.vf = 0;
    g_dtor_calls = 0; g_del_calls = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0,0);
    if (r != &obj || g_dtor_calls != 1 || g_del_calls != 0){ std::printf("FAIL noflag\n"); return 1; }
    g_dtor_calls = 0; g_del_calls = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0,1);
    if (r != &obj || g_dtor_calls != 1 || g_del_calls != 1){ std::printf("FAIL flag\n"); return 1; }
    g_dtor_calls = 0; g_del_calls = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0,2);
    if (r != &obj || g_dtor_calls != 1 || g_del_calls != 0){ std::printf("FAIL evenflag\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00456779_TEST PASS\n");
    return 0;
}