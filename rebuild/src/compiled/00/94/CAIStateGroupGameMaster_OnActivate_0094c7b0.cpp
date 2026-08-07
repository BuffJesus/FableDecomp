#include "rebuild_abi.h"
struct D { char p[0x1c9]; unsigned char f1c9; };
struct A { char p[0x18]; D* f18; };
struct C { char p[4]; A* f4; char pac[0xac-8]; unsigned char fac; void OnActivate(); };
void C::OnActivate(){ this->fac = (unsigned char)(this->f4->f18->f1c9 & 1); }