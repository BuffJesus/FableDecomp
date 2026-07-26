#include <cstdio>

static int g_dtor = 0;
static int g_del = 0;
static void __cdecl eng_operator_delete(void* p) { g_del++; (void)p; }

struct CLandscapeBackgroundPatch {
    void* vftable;
    int dtor_called;
    void base_dtor() { g_dtor++; dtor_called = 1; }
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags) {
        this->base_dtor();
        if (flags & 1)
            eng_operator_delete(this);
        return this;
    }
};

int main()
{
    CLandscapeBackgroundPatch a; a.vftable=0; a.dtor_called=0;
    void* r = a.CLandscapeBackgroundPatch_vector_deleting_destructor(0);
    if (r != &a || g_dtor != 1 || g_del != 0 || a.dtor_called != 1) { std::printf("FAIL noflag\n"); return 1; }

    CLandscapeBackgroundPatch b; b.vftable=0; b.dtor_called=0;
    r = b.CLandscapeBackgroundPatch_vector_deleting_destructor(1);
    if (r != &b || g_dtor != 2 || g_del != 1 || b.dtor_called != 1) { std::printf("FAIL flag\n"); return 1; }

    CLandscapeBackgroundPatch c; c.vftable=0; c.dtor_called=0;
    r = c.CLandscapeBackgroundPatch_vector_deleting_destructor(2);
    if (r != &c || g_del != 1) { std::printf("FAIL flag2\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00442cc0_TEST PASS\n");
    return 0;
}