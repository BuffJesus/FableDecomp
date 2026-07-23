#include <cstdio>

struct CLandscapeBackgroundPatch { void* vftable; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_calls++; }
void __cdecl eng_operator_delete(void* p) { (void)p; g_delete_calls++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        eng_operator_delete(self);
    }
    return self;
}

int main() {
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL noflag\n"); return 1; }
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj || g_dtor_calls != 2 || g_delete_calls != 1) { std::printf("FAIL flag\n"); return 1; }
    void* r3 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (r3 != &obj || g_dtor_calls != 3 || g_delete_calls != 1) { std::printf("FAIL evenflag\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00457ac8_TEST PASS\n");
    return 0;
}