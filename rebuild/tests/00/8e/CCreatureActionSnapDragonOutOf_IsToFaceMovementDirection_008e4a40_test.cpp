#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_8e4a40(void* self){ return 0; }
int main(){ int x; if(R_8e4a40(&x)!=0){std::printf("B1");return 1;} std::printf("RB_8e4a40_OK\n"); return 0; }