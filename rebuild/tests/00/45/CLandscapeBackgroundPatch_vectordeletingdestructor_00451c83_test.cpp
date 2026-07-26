#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;

struct CInner { int a; };
struct CLandscapeBackgroundPatch {
    int f0;
    CInner inner;
};

extern "C" void __fastcall inner_dtor(void* p) { (void)p; g_dtor_called++; }
void __cdecl op_delete(void* p) { (void)p; g_delete_called++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int, unsigned int flags)
{
    inner_dtor(&self->inner);
    if (flags & 1) {
        op_delete(self);
    }
    return self;
}

int main() {
    CLandscapeBackgroundPatch obj;
    obj.f0 = 0;
    // no delete
    g_dtor_called = 0; g_delete_called = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL nodelete r=%p dtor=%d del=%d\n", r, g_dtor_called, g_delete_called);
        return 1;
    }
    // with delete flag
    g_dtor_called = 0; g_delete_called = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) {
        std::printf("FAIL delete r=%p dtor=%d del=%d\n", r, g_dtor_called, g_delete_called);
        return 1;
    }
    std::printf("CLandscapeBackgroundPatch_00451c83_TEST PASS\n");
    return 0;
}