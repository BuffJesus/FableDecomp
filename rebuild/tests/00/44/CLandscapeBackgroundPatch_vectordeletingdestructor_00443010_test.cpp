#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;

void __cdecl eng_operator_delete(void* p) { (void)p; g_delete_called++; }

struct CLandscapeBackgroundPatch {
    void* vftable;
    void inner_dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void CLandscapeBackgroundPatch::inner_dtor() { g_dtor_called++; }

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->inner_dtor();
    if (flags & 1)
        eng_operator_delete(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;

    g_dtor_called = 0; g_delete_called = 0;
    void* r1 = obj.vector_deleting_destructor(0);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL case0\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0;
    void* r2 = obj.vector_deleting_destructor(1);
    if (r2 != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL case1\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0;
    void* r3 = obj.vector_deleting_destructor(3);
    if (r3 != &obj || g_delete_called != 1) { std::printf("FAIL case3\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00443010_TEST PASS\n");
    return 0;
}