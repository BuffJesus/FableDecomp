#include <cstdio>
// Behavioral reimplementation of the scalar-deleting-destructor half
// (the reachable path for the CLandscapeBackgroundPatch vector deleting dtor).
static int g_vtbl_a = 0x1232484, g_vtbl_b = 0x1230ba0;
static int dtor_called = 0, delete_called = 0;
static void dtor_fn(void*) { dtor_called = 1; }
static void delete_fn(void* p) { delete_called = 1; (void)p; }

static void* CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, unsigned int flags)
{
    *(int*)((char*)self + 0x28) = (int)&g_vtbl_a;
    *(int*)((char*)self + 0x0)  = (int)&g_vtbl_b;
    dtor_fn(self);
    if (flags & 1)
        delete_fn(self);
    return self;
}

int main()
{
    unsigned char buf[0x40];
    void* self = buf;
    dtor_called = delete_called = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(self, 1);
    if (r != self) { std::printf("FAIL: return\n"); return 1; }
    if (!dtor_called) { std::printf("FAIL: dtor\n"); return 1; }
    if (!delete_called) { std::printf("FAIL: delete not called with flag 1\n"); return 1; }
    if (*(int*)(buf + 0x28) != (int)&g_vtbl_a) { std::printf("FAIL: vtbl_a\n"); return 1; }
    if (*(int*)(buf + 0x0)  != (int)&g_vtbl_b) { std::printf("FAIL: vtbl_b\n"); return 1; }
    dtor_called = delete_called = 0;
    CLandscapeBackgroundPatch_vector_deleting_destructor(self, 0);
    if (delete_called) { std::printf("FAIL: delete called with flag 0\n"); return 1; }
    if (!dtor_called) { std::printf("FAIL: dtor flag0\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0044fd55_TEST PASS\n");
    return 0;
}