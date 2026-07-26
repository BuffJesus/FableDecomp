#include <cstdio>
#include <new>
struct CLandscapeBackgroundPatch { int f0; };
static int g_dtor_calls = 0;
void __fastcall CLandscapeBackgroundPatch_scalar_dtor(CLandscapeBackgroundPatch* self) { g_dtor_calls++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_scalar_dtor(self);
    if (flags & 1)
        ::operator delete(self);
    return self;
}

int main() {
    CLandscapeBackgroundPatch* obj = (CLandscapeBackgroundPatch*)::operator new(sizeof(CLandscapeBackgroundPatch));
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(obj, 0, 0);
    if (r1 != obj) { std::printf("FAIL ret1\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL dtor1\n"); return 1; }
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(obj, 0, 1);
    if (r2 != obj) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor_calls != 2) { std::printf("FAIL dtor2\n"); return 1; }
    CLandscapeBackgroundPatch* obj2 = (CLandscapeBackgroundPatch*)::operator new(sizeof(CLandscapeBackgroundPatch));
    void* r3 = CLandscapeBackgroundPatch_vector_deleting_destructor(obj2, 0, 3);
    if (r3 != obj2 || g_dtor_calls != 3) { std::printf("FAIL flags3\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00454a96_TEST PASS\n");
    return 0;
}