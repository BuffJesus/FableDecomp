#include <cstdio>
struct Sub;
static char* g_arg=0;
struct S { char pad[8]; void* ff; };
void __fastcall sub(Sub* p){ g_arg=(char*)p; }
void __fastcall DetachTarget(S* self){ if(self->ff) sub((Sub*)((char*)self+4)); }
int main(){ S o; o.ff=(void*)1; DetachTarget(&o);
 if(g_arg!=(char*)&o+4){ std::printf("00a10e10_TEST FAIL\n"); return 1;}
 std::printf("00a10e10_TEST PASS\n"); return 0;}