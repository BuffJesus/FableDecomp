#include "rebuild_abi.h"
#include <cstdio>
struct N { int f0; int* f4; };
extern "C" int FABLE_STDCALL FableRet_c623f0(N** p){ N* a=*p; int* d=a->f4; return *d; }
int main(){ int val=77; N a; a.f4=&val; N* pa=&a;
 if(FableRet_c623f0(&pa)!=77){std::printf("B1");return 1;}
 std::printf("RB_c623f0_OK\n"); return 0; }