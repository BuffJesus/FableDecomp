#include <cstdio>

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

struct CLandscapeBackgroundPatch {
    unsigned int vftable;
    char inner[0xb4];
    void* tex;
};

struct CInner;
static void __fastcall CLandscapeBackgroundPatch_dtor_inner(CInner* p) { (void)p; g_dtor_calls++; }
static void __cdecl engine_operator_delete(void* p) { (void)p; g_delete_calls++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, void* edx, unsigned int flags)
{
    (void)edx;
    self->vftable = 0x122e5b0;
    CLandscapeBackgroundPatch_dtor_inner((CInner*)(&self->inner));
    if (flags & 1)
        engine_operator_delete(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;

    // flags = 0: destruct only, no delete
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL: return not self\n"); return 1; }
    if (obj.vftable != 0x122e5b0) { std::printf("FAIL: vftable\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL: dtor not called\n"); return 1; }
    if (g_delete_calls != 0) { std::printf("FAIL: delete called with flags=0\n"); return 1; }

    // flags = 1: destruct + delete
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj) { std::printf("FAIL: return not self 2\n"); return 1; }
    if (g_dtor_calls != 2) { std::printf("FAIL: dtor not called 2\n"); return 1; }
    if (g_delete_calls != 1) { std::printf("FAIL: delete not called with flags=1\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00413480_TEST PASS\n");
    return 0;
}