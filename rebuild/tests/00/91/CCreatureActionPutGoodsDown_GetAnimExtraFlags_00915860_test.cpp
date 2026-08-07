#include "rebuild_abi.h"
#include <cstdio>
extern "C" int FABLE_FASTCALL R_915860(void* self){ return 8; }
int main(){ int x; if(R_915860(&x)!=8){std::printf("B1");return 1;} std::printf("RB_915860_OK\n"); return 0; }