#include "rebuild_abi.h"
#include <cstdio>
static int g_hit=0;
static void hit(){ g_hit=1; }
struct V { virtual void s0(){} virtual void s1(){} virtual void s2(){} virtual void s3(){} virtual void s4(){} virtual void s5(){} virtual void s6(){} virtual void s7(){} virtual void s8(){ hit(); } };
struct C { char pad[0x228]; V s; void Op(); };
void C::Op(){ this->s.s8(); }
int main(){ C o; V v; *(void**)&o.s = *(void**)&v; o.Op();
 if(!g_hit){std::printf("B1");return 1;}
 std::printf("RB_c21ea0_OK\n"); return 0; }