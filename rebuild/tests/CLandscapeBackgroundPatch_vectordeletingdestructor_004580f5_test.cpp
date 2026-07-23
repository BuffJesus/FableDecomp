#include <cstdio>
struct CLandscapeBackgroundPatch { void* vftable; int dtored; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void dtor_impl(CLandscapeBackgroundPatch* self){ g_dtor_called++; self->dtored = 1; }
static void delete_impl(void* p){ (void)p; g_delete_called++; }

static void* vdd(CLandscapeBackgroundPatch* self, unsigned int flags)
{
    dtor_impl(self);
    if (flags & 1)
        delete_impl(self);
    return self;
}

int main(){
    CLandscapeBackgroundPatch obj; obj.vftable=0; obj.dtored=0;
    g_dtor_called=0; g_delete_called=0;
    void* r = vdd(&obj, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0 || obj.dtored != 1){
        std::printf("FAIL flags=0 r=%p dtor=%d del=%d\n", r, g_dtor_called, g_delete_called);
        return 1;
    }
    obj.dtored=0; g_dtor_called=0; g_delete_called=0;
    r = vdd(&obj, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1 || obj.dtored != 1){
        std::printf("FAIL flags=1 r=%p dtor=%d del=%d\n", r, g_dtor_called, g_delete_called);
        return 1;
    }
    r = vdd(&obj, 2);
    if (g_delete_called != 1){ std::printf("FAIL flags=2 del=%d\n", g_delete_called); return 1; }
    std::printf("CLandscapeBackgroundPatch_004580f5_TEST PASS\n");
    return 0;
}