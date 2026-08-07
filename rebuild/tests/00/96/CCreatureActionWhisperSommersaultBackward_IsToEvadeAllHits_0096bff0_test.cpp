#include "rebuild_abi.h"
#include <cstdio>
struct C { char p[276]; unsigned char f; int R(); };
int C::R(){ return this->f==0; }
int main(){ C o; o.f=0; if(o.R()!=1){std::printf("B1");return 1;} o.f=5; if(o.R()!=0){std::printf("B2");return 1;} std::printf("RB_96bff0_OK\n"); return 0; }