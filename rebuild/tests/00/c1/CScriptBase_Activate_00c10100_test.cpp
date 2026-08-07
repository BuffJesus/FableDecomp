#include "rebuild_abi.h"
#include <cstdio>
static int a_hit=0,b_hit=0;
struct V { virtual void s0(){}virtual void s1(){}virtual void s2(){}virtual void s3(){}virtual void s4(){}virtual void s5(){}virtual void s6(){} };
struct DA : V { void s6(){ a_hit=1; } };
struct C : V { char p[0x10-4]; V s10; void Act(); };
void C::Act(){ this->s6(); this->s10.s6(); }
int main(){ C o; o.Act();  std::printf("RB_c10100_OK\n"); return 0; }