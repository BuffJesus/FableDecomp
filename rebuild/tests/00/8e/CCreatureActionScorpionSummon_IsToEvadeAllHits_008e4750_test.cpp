#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_8e4750(void* self){ return 1; }
int main(){ int x; if(R_8e4750(&x)!=1){std::printf("B1");return 1;} std::printf("RB_8e4750_OK\n"); return 0; }