#include <cstdio>

static int g_dtor_called = 0;
static int g_free_called = 0;
const void* CLandscapeBackgroundPatch_vftable = (const void*)0x1230ba0;

struct CLandscapeBackgroundPatch {
    const void** vft;
};

extern "C" void __cdecl FableFreeMem(void* p) { (void)p; g_free_called++; }

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    (void)edx;
    self->vft = (const void**)&CLandscapeBackgroundPatch_vftable;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        FableFreeMem(self);
    }
    return self;
}

int main() {
    CLandscapeBackgroundPatch obj;
    obj.vft = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0u);
    if (r != &obj) { std::printf("FAIL ret\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor\n"); return 1; }
    if (g_free_called != 0) { std::printf("FAIL free0\n"); return 1; }
    if (obj.vft != (const void**)&CLandscapeBackgroundPatch_vftable) { std::printf("FAIL vft\n"); return 1; }
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1u);
    if (r2 != &obj) { std::printf("FAIL ret2\n"); return 1; }
    if (g_free_called != 1) { std::printf("FAIL free1\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00450b79_TEST PASS\n");
    return 0;
}