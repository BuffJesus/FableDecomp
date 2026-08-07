#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_976fa0(void* self){ return 1; }
int main(){ int x; if(R_976fa0(&x)!=1){std::printf("B1");return 1;} std::printf("RB_976fa0_OK\n"); return 0; }