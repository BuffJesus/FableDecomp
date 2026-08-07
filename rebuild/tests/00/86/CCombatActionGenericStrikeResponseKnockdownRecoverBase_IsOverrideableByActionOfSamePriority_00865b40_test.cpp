#include <cstdio>
struct S; typedef unsigned char (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
static unsigned char __fastcall vm(S* p){ (void)p; return 0; }
static bool f(S* self){ return self->vtbl[0xb4/4](self) ? false : true; }
int main(){ VFn t[0x40]; for(int i=0;i<0x40;i++)t[i]=0; t[0xb4/4]=vm;
 S o; o.vtbl=t; if(!f(&o)){ std::printf("00865b40_TEST FAIL\n"); return 1;}
 std::printf("00865b40_TEST PASS\n"); return 0;}