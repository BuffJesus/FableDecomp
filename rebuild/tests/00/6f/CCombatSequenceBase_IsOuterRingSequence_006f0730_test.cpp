#include <cstdio>
struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
static int __fastcall vm(S* p){ (void)p; return 0x80; }
static bool f(S* self){ return self->vtbl[0x3c/4](self)==0x80; }
int main(){ VFn t[0x20]; for(int i=0;i<0x20;i++)t[i]=0; t[0x3c/4]=vm;
 S o; o.vtbl=t; if(!f(&o)){ std::printf("006f0730_TEST FAIL\n"); return 1;}
 std::printf("006f0730_TEST PASS\n"); return 0;}