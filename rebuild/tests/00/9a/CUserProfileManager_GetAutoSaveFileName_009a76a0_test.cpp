#include "rebuild_abi.h"
#include <cstdio>
char g_str_9a76a0=7; static int hit=0;
struct C { int x; void Helper(char* s); C* Get(); };
void C::Helper(char* s){ hit=1; }
C* C::Get(){ this->Helper(&g_str_9a76a0); return this; }
int main(){ C o; if(o.Get()!=&o){std::printf("B1");return 1;} if(!hit){std::printf("B2");return 1;} std::printf("RB_9a76a0_OK\n"); return 0; }