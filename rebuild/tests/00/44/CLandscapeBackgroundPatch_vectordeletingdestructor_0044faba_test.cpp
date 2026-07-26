#include <cstdio>
struct CLandscapeBackgroundPatch {
    void* vptr;
};
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_vft = (void*)0x1230ba0;
void CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
void engine_delete(void* p) { (void)p; g_delete_called++; }

void* CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, unsigned int flags)
{
    self->vptr = g_vft;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        engine_delete(self);
    }
    return self;
}

int main() {
    CLandscapeBackgroundPatch a; a.vptr = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0u);
    if (r != &a) { std::printf("FAIL return\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL delete0\n"); return 1; }
    if (a.vptr != g_vft) { std::printf("FAIL vptr\n"); return 1; }
    CLandscapeBackgroundPatch b; b.vptr = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&b, 1u);
    if (r != &b) { std::printf("FAIL return2\n"); return 1; }
    if (g_dtor_called != 2) { std::printf("FAIL dtor2\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL delete1\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0044faba_TEST PASS\n");
    return 0;
}