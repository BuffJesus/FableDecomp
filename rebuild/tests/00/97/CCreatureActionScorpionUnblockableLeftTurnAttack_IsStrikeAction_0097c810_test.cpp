#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_97c810(void* self){ return 1; }
int main(){ int x; if(R_97c810(&x)!=1){std::printf("B1");return 1;} std::printf("RB_97c810_OK\n"); return 0; }