#include <cstdio>

struct CLandscapeBackgroundPatch {
    int f0;
    void CLandscapeBackgroundPatch_dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

static int g_dtor_called = 0;
static int g_delete_called = 0;

void CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_dtor()
{
    g_dtor_called++;
}

extern "C" void __cdecl fable_operator_delete(void* p)
{
    g_delete_called++;
    (void)p;
}

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->CLandscapeBackgroundPatch_dtor();
    if (flags & 1)
        fable_operator_delete(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.f0 = 42;

    g_dtor_called = 0; g_delete_called = 0;
    void* r = obj.vector_deleting_destructor(0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL flags0 dtor=%d del=%d\n", g_dtor_called, g_delete_called);
        return 1;
    }

    g_dtor_called = 0; g_delete_called = 0;
    r = obj.vector_deleting_destructor(1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) {
        std::printf("FAIL flags1 dtor=%d del=%d\n", g_dtor_called, g_delete_called);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0045608d_TEST PASS\n");
    return 0;
}