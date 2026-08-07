#include "rebuild_abi.h"
#include <cstdio>
static int seen=-1;
struct C { virtual void s0(){} virtual void s1(){} virtual void s2(){} virtual void s3(){} virtual void s4(){} virtual void s5(){} virtual void s6(){} virtual void s7(){} virtual void s8(){} virtual void s9(){} virtual void s10(){} virtual void s11(){} virtual void s12(){} virtual void s13(){} virtual void s14(){} virtual void s15(){} virtual void s16(){} virtual void Write(void* p){ seen=*(int*)p; } void W(int a); };
void C::W(int a){ this->Write(&a); }
int main(){ C o; o.W(42); if(seen!=42){std::printf("B1");return 1;} std::printf("RB_c10140_OK\n"); return 0; }