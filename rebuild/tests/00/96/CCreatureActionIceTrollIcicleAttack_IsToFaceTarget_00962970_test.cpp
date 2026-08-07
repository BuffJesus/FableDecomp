#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_962970(void* self){ return 1; }
int main(){ int x; if(R_962970(&x)!=1){std::printf("B1");return 1;} std::printf("RB_962970_OK\n"); return 0; }