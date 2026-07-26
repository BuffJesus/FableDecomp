#include <cstdio>
struct CLandscapeBackgroundPatch { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
void __cdecl op_delete(void* p) { (void)p; g_delete_called++; }

struct CLBP_Vtbl {
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};

void* CLBP_Vtbl::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch* self = (CLandscapeBackgroundPatch*)this;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        op_delete(self);
    }
    return self;
}

int main() {
    CLandscapeBackgroundPatch obj;
    obj.f0 = 42;
    CLBP_Vtbl* h = (CLBP_Vtbl*)&obj;
    g_dtor_called = 0; g_delete_called = 0;
    void* r = h->CLandscapeBackgroundPatch_vector_deleting_destructor(0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL noflag\n"); return 1; }
    g_dtor_called = 0; g_delete_called = 0;
    r = h->CLandscapeBackgroundPatch_vector_deleting_destructor(1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL flag\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0045585a_TEST PASS\n");
    return 0;
}