#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_deleted_ptr = 0;

void __cdecl eng_operator_delete(void* p) { g_delete_called++; g_deleted_ptr = p; }

struct CLandscapeBackgroundPatch {
    void* vftable;
    void dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void CLandscapeBackgroundPatch::dtor() { g_dtor_called++; }

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->dtor();
    if (flags & 1)
        eng_operator_delete(this);
    return this;
}

int main() {
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;

    g_dtor_called = 0; g_delete_called = 0; g_deleted_ptr = 0;
    void* r0 = obj.vector_deleting_destructor(0);
    if (r0 != &obj) { std::printf("FAIL: return mismatch flags=0\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL: dtor not called flags=0\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL: delete called when flag clear\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0; g_deleted_ptr = 0;
    void* r1 = obj.vector_deleting_destructor(1);
    if (r1 != &obj) { std::printf("FAIL: return mismatch flags=1\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL: dtor not called flags=1\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL: delete not called flags=1\n"); return 1; }
    if (g_deleted_ptr != &obj) { std::printf("FAIL: wrong deleted ptr\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00457b56_TEST PASS\n");
    return 0;
}