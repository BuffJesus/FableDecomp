#include "rebuild_abi.h"
#include <cstdio>
static void* seen=(void*)-1;
extern "C" void FABLE_STDCALL Helper_9fc540(void* a){ seen=a; }
extern "C" void FABLE_STDCALL Handle_9fc540(void* a){ Helper_9fc540(0); }
int main(){ int x; Handle_9fc540(&x); if(seen!=0){std::printf("B1");return 1;} std::printf("RB_9fc540_OK\n"); return 0; }