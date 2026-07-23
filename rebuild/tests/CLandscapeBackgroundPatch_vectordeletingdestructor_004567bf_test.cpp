#include <cstdio>

extern void __cdecl operator_delete(void* p);

struct CLandscapeBackgroundPatch {
    int f0;
    void __fastcall scalar_dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

static int g_dtor_called = 0;
static int g_delete_called = 0;
void __fastcall CLandscapeBackgroundPatch::scalar_dtor() { g_dtor_called++; }
void __cdecl operator_delete(void* p) { g_delete_called++; (void)p; }

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->scalar_dtor();
    if (flags & 1)
        operator_delete(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.f0 = 42;
    void* r = obj.vector_deleting_destructor(0);
    if (r != &obj) { std::printf("FAIL ret mismatch\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor not called\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL delete called when flag=0\n"); return 1; }
    r = obj.vector_deleting_destructor(1);
    if (r != &obj) { std::printf("FAIL ret mismatch2\n"); return 1; }
    if (g_dtor_called != 2) { std::printf("FAIL dtor not called2\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL delete not called when flag=1\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_004567bf_TEST PASS\n");
    return 0;
}