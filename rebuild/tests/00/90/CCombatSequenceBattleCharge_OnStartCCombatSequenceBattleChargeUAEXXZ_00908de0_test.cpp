#include "rebuild_abi.h"
#include <cstdio>
struct G { int a; int Helper(); };
int G::Helper(){ return this->a+3; }
static G go; G* g_908de0=&go;
struct C { char pad[0x28]; int f28; void OnStart(); };
void C::OnStart(){ this->f28 = g_908de0->Helper(); }
int main(){ go.a=4; C o; o.OnStart(); if(o.f28!=7){std::printf("B1");return 1;} std::printf("RB_908de0_OK\n"); return 0; }