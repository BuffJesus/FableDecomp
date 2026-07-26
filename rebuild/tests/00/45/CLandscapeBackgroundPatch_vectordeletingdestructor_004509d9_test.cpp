#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_dtor_arg = 0;
static void* g_delete_arg = 0;

void __fastcall Sub_Dtor(void* p) { g_dtor_called++; g_dtor_arg = p; }
void __cdecl Op_Delete(void* p) { g_delete_called++; g_delete_arg = p; }

struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    Sub_Dtor((char*)self + 4);
    if (flags & 1) {
        Op_Delete(self);
    }
    return self;
}

int main() {
    CLandscapeBackgroundPatch obj;
    obj.f0 = 0; obj.f4 = 0; obj.f8 = 0;

    // flags without bit0: destructor called, no delete
    g_dtor_called = g_delete_called = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0 || g_dtor_arg != (char*)&obj + 4) {
        std::printf("FAIL case0\n"); return 1;
    }

    // flags with bit0: destructor + delete
    g_dtor_called = g_delete_called = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1 || g_delete_arg != &obj) {
        std::printf("FAIL case1\n"); return 1;
    }

    std::printf("CLandscapeBackgroundPatch_004509d9_TEST PASS\n");
    return 0;
}