#include <cstdio>
struct S; typedef unsigned char (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
static unsigned char __fastcall vm(S* p){ (void)p; return 1; }
static bool f(S* self){ return self->vtbl[0xC4/4](self) ? true : false; }
int main(){ VFn t[0x40]; for(int i=0;i<0x40;i++)t[i]=0; t[0xC4/4]=vm;
 S obj; obj.vtbl=t; if(!f(&obj)){ std::printf("00692670_TEST FAIL\n"); return 1;}
 std::printf("00692670_TEST PASS\n"); return 0;}