#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_96c080(void* self){ return 1; }
int main(){ int x; if(R_96c080(&x)!=1){std::printf("B1");return 1;} std::printf("RB_96c080_OK\n"); return 0; }