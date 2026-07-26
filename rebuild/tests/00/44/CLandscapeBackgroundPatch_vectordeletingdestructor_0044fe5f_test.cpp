#include <cstdio>
struct CLandscapeBackgroundPatch { void* vptr; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_vtbl_addr = (void*)0x1230ba0;
void CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch*) { g_dtor_called++; }
void op_delete(void*) { g_delete_called++; }

void* mydel(CLandscapeBackgroundPatch* self, unsigned int flags)
{
    self->vptr = g_vtbl_addr;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) op_delete(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch o; o.vptr = 0;
    void* r = mydel(&o, 1);
    if (r != &o) { std::printf("FAIL ret\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL del\n"); return 1; }
    if (o.vptr != g_vtbl_addr) { std::printf("FAIL vptr\n"); return 1; }
    g_delete_called = 0;
    mydel(&o, 0);
    if (g_delete_called != 0) { std::printf("FAIL nodel\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0044fe5f_TEST PASS\n");
    return 0;
}