#include "rebuild_abi.h"
#include <cstdio>
struct R { char p[0x14]; float f14; };
struct Sub { R* Get(void* a); };
static R r;
R* Sub::Get(void* a){ r.f14=2.5f; return &r; }
struct C { char p[0x20]; Sub s20; float Set(void* a); };
float C::Set(void* a){ return this->s20.Get(a)->f14; }
int main(){ C o; int x; if(o.Set(&x)!=2.5f){std::printf("B1");return 1;} std::printf("RB_a4eae0_OK\n"); return 0; }