#include <cstdio>

struct CLandscapeBackgroundPatch { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_deleted = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
void __cdecl eng_operator_delete(void* p) { g_delete_called++; g_deleted = p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        eng_operator_delete(self);
    }
    return self;
}

int main() {
    CLandscapeBackgroundPatch obj; obj.f0 = 7;
    // flags without bit0: dtor only, no delete
    g_dtor_called = g_delete_called = 0; g_deleted = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL case0\n"); return 1; }
    // flags with bit0: dtor + delete
    g_dtor_called = g_delete_called = 0; g_deleted = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj || g_dtor_called != 1 || g_delete_called != 1 || g_deleted != &obj) { std::printf("FAIL case1\n"); return 1; }
    // even flag bits without bit0 -> no delete
    g_dtor_called = g_delete_called = 0;
    void* r3 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (r3 != &obj || g_delete_called != 0) { std::printf("FAIL case2\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_004575e0_TEST PASS\n");
    return 0;
}