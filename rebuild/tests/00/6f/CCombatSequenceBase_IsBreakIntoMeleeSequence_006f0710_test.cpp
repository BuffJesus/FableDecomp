#include <cstdio>
struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
static int __fastcall vm(S* p){ (void)p; return 2; }
static bool f(S* self){ return self->vtbl[0xc/4](self)==2; }
int main(){ VFn t[0x10]; for(int i=0;i<0x10;i++)t[i]=0; t[0xc/4]=vm;
 S obj; obj.vtbl=t; if(!f(&obj)){ std::printf("006f0710_TEST FAIL\n"); return 1;}
 std::printf("006f0710_TEST PASS\n"); return 0;}