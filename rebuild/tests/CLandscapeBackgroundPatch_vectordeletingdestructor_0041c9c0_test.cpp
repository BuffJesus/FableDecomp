#include <cstdio>
static int g_deleted = 0;
static int g_dtored = 0;
struct CLandscapeBackgroundPatch { void* vftable; };
static void* op_delete(void* p) { g_deleted = 1; return p; }
static const void* g_vft = (const void*)0x122f84c;
static void dtor(CLandscapeBackgroundPatch* self) { g_dtored = 1; }
static void* vdd(CLandscapeBackgroundPatch* self, char flags)
{
    self->vftable = (void*)&g_vft;
    dtor(self);
    if (flags & 1) { op_delete(self); }
    return self;
}
int main() {
    CLandscapeBackgroundPatch obj; obj.vftable = 0;
    void* r = vdd(&obj, 0);
    if (r != &obj || obj.vftable != (void*)&g_vft || !g_dtored || g_deleted) { std::printf("FAIL nodelete\n"); return 1; }
    g_dtored = 0;
    r = vdd(&obj, 1);
    if (r != &obj || !g_dtored || !g_deleted) { std::printf("FAIL delete\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0041c9c0_TEST PASS\n");
    return 0;
}