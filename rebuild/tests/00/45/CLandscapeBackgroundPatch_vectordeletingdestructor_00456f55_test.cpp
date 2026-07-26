#include <cstdio>

struct CLandscapeBackgroundPatch {
    int f0;
};

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) {
    (void)self;
    ++g_dtor_calls;
}
extern "C" void __cdecl op_delete(void* p) {
    (void)p;
    ++g_delete_calls;
}

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        op_delete(self);
    }
    return self;
}

int main() {
    CLandscapeBackgroundPatch obj;
    obj.f0 = 42;

    // flags with bit0 set => destructor + delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r1 != &obj || g_dtor_calls != 1 || g_delete_calls != 1) {
        std::printf("FAIL case1 r=%p dtor=%d del=%d\n", r1, g_dtor_calls, g_delete_calls);
        return 1;
    }

    // flags without bit0 => destructor only, no delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r2 != &obj || g_dtor_calls != 1 || g_delete_calls != 0) {
        std::printf("FAIL case2 r=%p dtor=%d del=%d\n", r2, g_dtor_calls, g_delete_calls);
        return 1;
    }

    // even flag with bit0 clear (2) => no delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r3 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (r3 != &obj || g_delete_calls != 0) {
        std::printf("FAIL case3\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00456f55_TEST PASS\n");
    return 0;
}