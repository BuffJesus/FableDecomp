#include "rebuild_abi.h"
#include <cstdio>
static int hit=0;
extern "C" void FABLE_STDCALL Helper_68c6d0(void* a){ hit=1; }
extern "C" void FABLE_STDCALL Proc_68c6d0(void* a, void* b){ Helper_68c6d0(a); }
int main(){ int x,y; Proc_68c6d0(&x,&y); if(!hit){std::printf("B1");return 1;} std::printf("RB_68c6d0_OK\n"); return 0; }