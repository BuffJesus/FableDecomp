#include "rebuild_abi.h"
#include <cstdio>
extern "C" char FABLE_FASTCALL R_956850(void* self){ return 1; }
int main(){ int x; if(R_956850(&x)!=1){std::printf("B1");return 1;} std::printf("RB_956850_OK\n"); return 0; }