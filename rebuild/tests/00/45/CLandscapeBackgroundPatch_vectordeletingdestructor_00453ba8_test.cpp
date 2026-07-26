#include <cstdio>

struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    void* vector_deleting_destructor(unsigned int flags);
};

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_deleted = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { g_dtor_called++; }
void __cdecl eng_operator_delete(void* p) { g_delete_called++; g_deleted = p; }

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(this);
    if (flags & 1)
        eng_operator_delete(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    g_dtor_called = 0; g_delete_called = 0; g_deleted = 0;
    void* r = obj.vector_deleting_destructor(0);
    if (r != &obj) { std::printf("FAIL: return\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL: dtor not called\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL: delete called on flag0\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0; g_deleted = 0;
    r = obj.vector_deleting_destructor(1);
    if (r != &obj) { std::printf("FAIL: return2\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL: dtor not called2\n"); return 1; }
    if (g_delete_called != 1 || g_deleted != &obj) { std::printf("FAIL: delete not called on flag1\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00453ba8_TEST PASS\n");
    return 0;
}