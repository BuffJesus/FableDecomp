#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_953240(void* self){ return 0; }
int main(){ int x; if(R_953240(&x)!=0){std::printf("B1");return 1;} std::printf("RB_953240_OK\n"); return 0; }