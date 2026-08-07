#include "rebuild_abi.h"
#include <cstdio>
struct D { char p[0x1c9]; unsigned char f1c9; };
struct A { char p[0x18]; D* f18; };
struct C { char p[4]; A* f4; char pac[0xac-8]; unsigned char fac; void OnActivate(); };
void C::OnActivate(){ this->fac = (unsigned char)(this->f4->f18->f1c9 & 1); }
int main(){ D d; d.f1c9=0x03; A a; a.f18=&d; C o; o.f4=&a; o.fac=0; o.OnActivate(); if(o.fac!=1){std::printf("B1");return 1;} d.f1c9=0x02; o.OnActivate(); if(o.fac!=0){std::printf("B2");return 1;} std::printf("RB_94c7b0_OK\n"); return 0; }