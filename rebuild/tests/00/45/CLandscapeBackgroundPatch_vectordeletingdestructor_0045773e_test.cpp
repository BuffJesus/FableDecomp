#include <cstdio>

struct CLandscapeBackgroundPatch {
    void* vftable;
    void Dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_deleted_ptr = 0;

void CLandscapeBackgroundPatch::Dtor() { g_dtor_called++; }
void __cdecl op_delete(void* p) { g_delete_called++; g_deleted_ptr = p; }

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->Dtor();
    if (flags & 1)
        op_delete(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch obj; obj.vftable = 0;

    g_dtor_called = 0; g_delete_called = 0; g_deleted_ptr = 0;
    void* r0 = obj.vector_deleting_destructor(0);
    if (r0 != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL flags=0\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0; g_deleted_ptr = 0;
    void* r1 = obj.vector_deleting_destructor(1);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 1 || g_deleted_ptr != &obj) { std::printf("FAIL flags=1\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0;
    void* r2 = obj.vector_deleting_destructor(2);
    if (r2 != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL flags=2\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_0045773e_TEST PASS\n");
    return 0;
}