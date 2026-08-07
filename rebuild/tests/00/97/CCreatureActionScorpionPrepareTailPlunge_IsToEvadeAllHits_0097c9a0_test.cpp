#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_97c9a0(void* self){ return 1; }
int main(){ int x; if(R_97c9a0(&x)!=1){std::printf("B1");return 1;} std::printf("RB_97c9a0_OK\n"); return 0; }