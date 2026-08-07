#include "rebuild_abi.h"
#include <cstdio>
static int hit=0; extern "C" void FABLE_CDECL Imp_c06050(void* p){ hit=1; }
extern "C" void FABLE_CDECL Fwd_c06050(void* a, void* b){ Imp_c06050(b); }
int main(){ int x,y; Fwd_c06050(&x,&y); if(!hit){std::printf("B1");return 1;} std::printf("RB_c06050_OK\n"); return 0; }