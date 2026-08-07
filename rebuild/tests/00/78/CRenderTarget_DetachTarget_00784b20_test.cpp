#include "rebuild_abi.h"
#include <cstdio>
struct C { char p[0x2c]; void* f2c; void Detach(); void Helper(); };
static int hit=0; void C::Helper(){ hit=1; }
void C::Detach(){ if(this->f2c) this->Helper(); }
int main(){ C o; int x; o.f2c=&x; o.Detach(); if(!hit){std::printf("B1");return 1;} hit=0; o.f2c=0; o.Detach(); if(hit){std::printf("B2");return 1;} std::printf("RB_784b20_OK\n"); return 0; }