#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_961a30(void* self){ return 1; }
int main(){ int x; if(R_961a30(&x)!=1){std::printf("B1");return 1;} std::printf("RB_961a30_OK\n"); return 0; }