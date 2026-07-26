#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void *g_deleted = 0;

void __cdecl engine_operator_delete(void *p)
{
    g_delete_called++;
    g_deleted = p;
}

struct CLandscapeBackgroundPatch {
    void *vptr;
    void real_dtor() { g_dtor_called++; }
    void * CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};

void * CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    this->real_dtor();
    if (flags & 1)
        engine_operator_delete(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.vptr = 0;

    g_dtor_called = 0; g_delete_called = 0; g_deleted = 0;
    void *r0 = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(0);
    if (r0 != &obj || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL flags=0 path\n");
        return 1;
    }

    g_dtor_called = 0; g_delete_called = 0; g_deleted = 0;
    void *r1 = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(1);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 1 || g_deleted != &obj) {
        std::printf("FAIL flags=1 path\n");
        return 1;
    }

    g_dtor_called = 0; g_delete_called = 0;
    void *r2 = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(2);
    if (r2 != &obj || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL flags=2 path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0044f7fe_TEST PASS\n");
    return 0;
}