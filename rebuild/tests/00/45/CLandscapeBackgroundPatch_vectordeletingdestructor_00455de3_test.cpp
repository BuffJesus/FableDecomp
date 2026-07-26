#include <cstdio>
#include <cstdlib>

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    void* vector_deleting_destructor(unsigned int flags);
};

static void __fastcall CLandscapeBackgroundPatch_dtor(void* self) { (void)self; ++g_dtor_calls; }

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(this);
    if (flags & 1) {
        ++g_delete_calls;
    }
    return this;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.f0 = 0x11223344;

    g_dtor_calls = 0; g_delete_calls = 0;
    void* r = obj.vector_deleting_destructor(0);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) {
        std::printf("FAIL flags=0 dtor=%d del=%d r=%p\n", g_dtor_calls, g_delete_calls, r);
        return 1;
    }

    g_dtor_calls = 0; g_delete_calls = 0;
    r = obj.vector_deleting_destructor(1);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 1) {
        std::printf("FAIL flags=1 dtor=%d del=%d r=%p\n", g_dtor_calls, g_delete_calls, r);
        return 1;
    }

    g_dtor_calls = 0; g_delete_calls = 0;
    r = obj.vector_deleting_destructor(2);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) {
        std::printf("FAIL flags=2 dtor=%d del=%d r=%p\n", g_dtor_calls, g_delete_calls, r);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00455de3_TEST PASS\n");
    return 0;
}