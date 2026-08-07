#include <cstdio>
struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
static int __fastcall vm(S* p){ (void)p; return 3; }
static bool f(S* self){ return self->vtbl[0x174/4](self)!=0; }
int main(){ VFn t[0x100]; for(int i=0;i<0x100;i++)t[i]=0; t[0x174/4]=vm;
 S o; o.vtbl=t; if(!f(&o)){ std::printf("006afd30_TEST FAIL\n"); return 1;}
 std::printf("006afd30_TEST PASS\n"); return 0;}