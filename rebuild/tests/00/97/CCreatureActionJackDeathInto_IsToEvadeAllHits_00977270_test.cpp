#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_977270(void* self){ return 1; }
int main(){ int x; if(R_977270(&x)!=1){std::printf("B1");return 1;} std::printf("RB_977270_OK\n"); return 0; }