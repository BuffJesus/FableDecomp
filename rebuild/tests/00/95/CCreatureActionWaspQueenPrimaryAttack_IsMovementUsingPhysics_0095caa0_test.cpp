#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_95caa0(void* self){ return 0; }
int main(){ int x; if(R_95caa0(&x)!=0){std::printf("B1");return 1;} std::printf("RB_95caa0_OK\n"); return 0; }