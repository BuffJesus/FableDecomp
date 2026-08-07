#include "rebuild_abi.h"
struct V {
 virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4(); virtual void s5(); virtual void s6();
};
struct C : V { char p[0x10-4]; V s10; void Act(); };
void C::Act(){ this->s6(); this->s10.s6(); }