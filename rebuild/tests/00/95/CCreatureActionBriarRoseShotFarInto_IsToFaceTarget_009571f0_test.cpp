#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_9571f0(void* self){ return 1; }
int main(){ int x; if(R_9571f0(&x)!=1){std::printf("B1");return 1;} std::printf("RB_9571f0_OK\n"); return 0; }