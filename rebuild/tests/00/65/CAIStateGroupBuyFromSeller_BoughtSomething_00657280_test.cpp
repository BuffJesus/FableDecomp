#include "rebuild_abi.h"
#include <cstdio>
struct G { int a; int Helper(); };
int G::Helper(){ return this->a+3; }
static G go; G* g_657280=&go;
struct C { char pad[0x2c]; int f2c; void Bought(); };
void C::Bought(){ this->f2c = g_657280->Helper(); }
int main(){ go.a=4; C o; o.Bought(); if(o.f2c!=7){std::printf("B1");return 1;} std::printf("RB_657280_OK\n"); return 0; }