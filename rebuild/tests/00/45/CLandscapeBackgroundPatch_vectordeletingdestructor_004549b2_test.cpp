#include <cstdio>
static int g_dtor_called = 0;
static int g_delete_called = 0;
struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    char inner[4];
    unsigned char _pad_0x10[0xa8];
    void* tex;
    ~CLandscapeBackgroundPatch() { g_dtor_called++; }
};
extern "C" void __cdecl fable_operator_delete(void* p) { (void)p; g_delete_called++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    self->~CLandscapeBackgroundPatch();
    if (flags & 1) {
        fable_operator_delete(self);
    }
    return self;
}

int main() {
    CLandscapeBackgroundPatch obj;
    g_dtor_called = 0; g_delete_called = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL: return not self\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL: dtor count %d\n", g_dtor_called); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL: delete should not be called for flags=0\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (g_dtor_called != 1) { std::printf("FAIL: dtor count2 %d\n", g_dtor_called); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL: delete should be called for flags=1\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_004549b2_TEST PASS\n");
    return 0;
}