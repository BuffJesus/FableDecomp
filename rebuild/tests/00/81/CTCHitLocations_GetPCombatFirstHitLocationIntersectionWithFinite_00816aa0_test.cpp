#include "rebuild_abi.h"
#include <cstdio>
static int g_hit=0;
extern "C" void FABLE_FASTCALL FableImpl_816aa0(void *self){ g_hit=1; }
extern "C" void FABLE_FASTCALL FableThunk_816aa0(void *self){ FableImpl_816aa0(self); }
int main(){ int x; FableThunk_816aa0(&x);
 if(!g_hit){std::printf("B1\n");return 1;}
 std::printf("TH_816aa0_OK\n"); return 0; }