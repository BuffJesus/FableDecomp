#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_deleted = 0;

void __cdecl fable_operator_delete(void* p) {
    g_delete_called++;
    g_deleted = p;
}

struct CLandscapeBackgroundPatch {
    void* vftable;
    void real_dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void CLandscapeBackgroundPatch::real_dtor() {
    g_dtor_called++;
}

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->real_dtor();
    if (flags & 1) {
        fable_operator_delete(this);
    }
    return this;
}

int main() {
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;

    g_dtor_called = 0; g_delete_called = 0; g_deleted = 0;
    void* r1 = obj.vector_deleting_destructor(0);
    if (r1 != &obj) { std::printf("FAIL: return mismatch flags=0\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL: dtor not called flags=0\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL: delete called flags=0\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0; g_deleted = 0;
    void* r2 = obj.vector_deleting_destructor(1);
    if (r2 != &obj) { std::printf("FAIL: return mismatch flags=1\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL: dtor not called flags=1\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL: delete not called flags=1\n"); return 1; }
    if (g_deleted != &obj) { std::printf("FAIL: delete wrong ptr\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00458017_TEST PASS\n");
    return 0;
}