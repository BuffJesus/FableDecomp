#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_953590(void* self){ return 0; }
int main(){ int x; if(R_953590(&x)!=0){std::printf("B1");return 1;} std::printf("RB_953590_OK\n"); return 0; }