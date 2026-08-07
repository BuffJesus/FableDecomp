#include "rebuild_abi.h"
#include <cstdio>
extern "C" void FABLE_FASTCALL Act_940bd0_A(){}
static int bv=0; extern "C" int FABLE_FASTCALL Act_940bd0_B(){ return bv; }
struct C { char p[0x30]; int f30; char p2[0x3c-0x34]; int f3c; void OnActivate(); };
void C::OnActivate(){ this->f3c++; Act_940bd0_A(); this->f30 = Act_940bd0_B(); }
int main(){ C o; o.f3c=1; o.f30=0; bv=7; o.OnActivate(); if(o.f3c!=2||o.f30!=7){std::printf("B1");return 1;} std::printf("RB_940bd0_OK\n"); return 0; }