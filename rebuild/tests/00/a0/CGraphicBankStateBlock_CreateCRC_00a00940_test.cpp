#include "rebuild_abi.h"
#include <cstdio>
struct C { int f0; int f4; int Helper(void* p, int n); int CreateCRC(); };
int C::Helper(void* p, int n){ return n + *(int*)p; }
int C::CreateCRC(){ return this->Helper(&this->f4, 0x60); }
int main(){ C o; o.f4=5; if(o.CreateCRC()!=0x60+5){std::printf("B1");return 1;} std::printf("RB_a00940_OK\n"); return 0; }