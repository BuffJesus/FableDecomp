
#include <cstdio>
static int g_deleted, g_dtor;
static int vtbl_a=0;
extern "C" void eng_delete(void* p){ g_deleted=1; }
extern "C" void __fastcall CLBP_dtor(void* self){ g_dtor=1; }
struct S{ int v; };
static void* dtor(S* self, unsigned int flags)
{
    *(int*)self = (int)&vtbl_a;
    CLBP_dtor(self);
    if (flags & 1) { eng_delete(self); }
    return self;
}
int main(){
    S o; o.v=123;
    g_deleted=0; g_dtor=0;
    void* r = dtor(&o, 0);
    if (r != &o || g_dtor!=1 || g_deleted!=0){ std::printf("FAIL a\n"); return 1; }
    g_deleted=0; g_dtor=0;
    r = dtor(&o, 1);
    if (r != &o || g_dtor!=1 || g_deleted!=1){ std::printf("FAIL b\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0044feeb_TEST PASS\n");
    return 0;
}