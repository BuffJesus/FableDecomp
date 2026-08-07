#include <cstdio>
static void* g_p=(void*)1; static int g_z=-1;
static void __stdcall pfl_helper(void* p, int z){ g_p=p; g_z=z; }
struct S { char pad[0x9c]; void* f9c; };
static void PrepareForLoad(S* self){ pfl_helper(self->f9c, 0); }
int main(){ int m; S o; o.f9c=&m; PrepareForLoad(&o);
 if(g_p!=&m||g_z!=0){ std::printf("004ff560_TEST FAIL\n"); return 1;}
 std::printf("004ff560_TEST PASS\n"); return 0;}