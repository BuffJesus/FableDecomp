#include <cstdio>

struct CLandscapeBackgroundPatch { void* vftable; };

static int g_dtor = 0;
static int g_delete = 0;
static void* g_last = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { g_dtor++; }
void __cdecl operator delete(void* p) { g_delete++; g_last = p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_pad, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator delete((void*)self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor != 1 || g_delete != 0) { std::printf("FAIL a\n"); return 1; }
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor != 2 || g_delete != 1 || g_last != &obj) { std::printf("FAIL b\n"); return 1; }
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (r != &obj || g_dtor != 3 || g_delete != 1) { std::printf("FAIL c\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00457c84_TEST PASS\n");
    return 0;
}