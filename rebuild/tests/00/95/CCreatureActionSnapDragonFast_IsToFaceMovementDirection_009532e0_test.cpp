#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_9532e0(void* self){ return 0; }
int main(){ int x; if(R_9532e0(&x)!=0){std::printf("B1");return 1;} std::printf("RB_9532e0_OK\n"); return 0; }