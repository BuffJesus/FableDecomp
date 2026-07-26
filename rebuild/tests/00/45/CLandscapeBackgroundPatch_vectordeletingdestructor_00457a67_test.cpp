#include <cstdio>

struct CLandscapeBackgroundPatch { void* vftable; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_calls++; }
void operator_delete_stub(void* p) { (void)p; g_delete_calls++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        operator_delete_stub(self);
    }
    return self;
}

int main() {
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;
    // flags without bit 0: no delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL nodelete\n"); return 1; }
    // flags with bit 0: delete
    g_dtor_calls = 0; g_delete_calls = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 1) { std::printf("FAIL delete\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00457a67_TEST PASS\n");
    return 0;
}