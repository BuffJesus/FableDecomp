#include <cstdio>

struct CLandscapeBackgroundPatch { void* vftable; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; ++g_dtor_calls; }
void __cdecl op_delete(void* p) { (void)p; ++g_delete_calls; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main() {
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL no-delete\n"); return 1; }
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_calls != 2 || g_delete_calls != 1) { std::printf("FAIL delete\n"); return 1; }
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 2);
    if (g_delete_calls != 1) { std::printf("FAIL bit-mask\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00457de7_TEST PASS\n");
    return 0;
}