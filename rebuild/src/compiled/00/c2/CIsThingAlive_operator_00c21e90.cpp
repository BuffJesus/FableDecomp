#include "rebuild_abi.h"
struct V { virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7(); virtual void s8(); virtual void s9(); };
struct C { char p[0x228]; V s228; void Op(); };
void C::Op(){ this->s228.s9(); }