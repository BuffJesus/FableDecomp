#include <cstdio>
struct OF { char pad[8]; int f8; };
static OF* gP; void __stdcall h_b2fac0(OF* p){ gP=p; }
int __fastcall fn_00b2fac0(void* self, int _edx, OF* arg){ h_b2fac0(arg); return arg->f8; }
int main(){ OF o; o.f8=77; if(fn_00b2fac0(0,0,&o)!=77||gP!=&o){ printf("00b2fac0_TEST FAIL\n"); return 1; }
 printf("00b2fac0_TEST PASS\n"); return 0; }