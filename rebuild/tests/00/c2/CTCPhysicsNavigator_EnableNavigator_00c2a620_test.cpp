#include "rebuild_abi.h"
#include <cstdio>
int g_vt_c2a620=0;
struct C { void* vt; char f4; C* Enable(char a); };
C* C::Enable(char a){ this->vt=&g_vt_c2a620; this->f4=a; return this; }
int main(){ C o; o.f4=0; if(o.Enable(5)!=&o){std::printf("B1");return 1;} if(o.vt!=(void*)&g_vt_c2a620||o.f4!=5){std::printf("B2");return 1;} std::printf("RB_c2a620_OK\n"); return 0; }