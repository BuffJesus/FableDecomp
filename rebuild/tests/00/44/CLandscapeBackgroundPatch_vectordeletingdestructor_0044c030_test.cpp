#include <cstdio>
static int g_dtor_calls = 0;
static int g_delete_calls = 0;
extern void __cdecl engine_operator_delete(void* p) { (void)p; ++g_delete_calls; }

struct CLandscapeBackgroundPatch {
    void* vftable;
    void RealDtor() { ++g_dtor_calls; }
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    this->RealDtor();
    if (flags & 1)
        engine_operator_delete(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch obj; obj.vftable = 0;
    void* r1 = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(0);
    if (r1 != &obj) { std::printf("FAIL: ret mismatch flag0\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL: dtor not called flag0\n"); return 1; }
    if (g_delete_calls != 0) { std::printf("FAIL: delete called on flag0\n"); return 1; }
    void* r2 = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(1);
    if (r2 != &obj) { std::printf("FAIL: ret mismatch flag1\n"); return 1; }
    if (g_dtor_calls != 2) { std::printf("FAIL: dtor not called flag1\n"); return 1; }
    if (g_delete_calls != 1) { std::printf("FAIL: delete not called flag1\n"); return 1; }
    void* r3 = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(2);
    if (g_delete_calls != 1) { std::printf("FAIL: delete called on even flag\n"); return 1; }
    (void)r3;
    std::printf("CLandscapeBackgroundPatch_0044c030_TEST PASS\n");
    return 0;
}