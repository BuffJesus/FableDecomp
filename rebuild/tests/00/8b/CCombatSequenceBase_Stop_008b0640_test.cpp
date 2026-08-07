#include "rebuild_abi.h"
#include <cstdio>
static int g_hit=0; static void hit(){ g_hit=1; }
struct C { virtual void s0(){} virtual void s1(){} virtual void s2(){} virtual void s3(){} virtual void s4(){} virtual void s5(){} virtual void s6(){} virtual void s7(){} virtual void s8(){} virtual void s9(){} virtual void s10(){} virtual void s11(){} virtual void s12(){} virtual void s13(){} virtual void s14(){} virtual void s15(){} virtual void s16(){} virtual void s17(){} virtual void s18(){ hit(); } char f4; char pad[0xc-5]; int fc; void Stop(); };
void C::Stop(){ this->f4=0; this->fc=0; this->s18(); }
int main(){ C o; o.f4=9; o.fc=9; o.Stop(); if(o.f4!=0||o.fc!=0||!g_hit){std::printf("B1");return 1;} std::printf("RB_8b0640_OK\n"); return 0; }