#include "rebuild_abi.h"
#include <cstdio>
int g_vt_c1eb10=0;
struct Sub { void M(void* p); };
static void* seen=0; void Sub::M(void* p){ seen=p; }
struct C { void* vt; Sub* f4; void Op(); };
void C::Op(){ this->vt=&g_vt_c1eb10; Sub* s=this->f4; ((Sub*)((char*)s+0x2c))->M(this); }
int main(){ char buf[0x40]; C o; o.f4=(Sub*)buf; o.Op(); if(o.vt!=(void*)&g_vt_c1eb10||seen!=&o){std::printf("B1");return 1;} std::printf("RB_c1eb10_OK\n"); return 0; }