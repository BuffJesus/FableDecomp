#include "rebuild_abi.h"
struct S { virtual void s0(){} virtual void s1(){} virtual void s2(){} virtual void s3(){} virtual void s4(){} virtual void s5(){} virtual void s6(){} virtual void s7(){} virtual void s8(){} virtual void s9(){} virtual void s10(){} virtual void s11(){} virtual void s12(){} virtual void s13(){} virtual void s14(){} virtual void Ask(int a){} };
struct C { char p[0xc]; S* fc; void Ask(int a); };
void C::Ask(int a){ if(this->fc) this->fc->Ask(a); }