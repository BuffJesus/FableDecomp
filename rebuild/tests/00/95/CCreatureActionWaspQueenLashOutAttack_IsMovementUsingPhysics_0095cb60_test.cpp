#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_95cb60(void* self){ return 0; }
int main(){ int x; if(R_95cb60(&x)!=0){std::printf("B1");return 1;} std::printf("RB_95cb60_OK\n"); return 0; }