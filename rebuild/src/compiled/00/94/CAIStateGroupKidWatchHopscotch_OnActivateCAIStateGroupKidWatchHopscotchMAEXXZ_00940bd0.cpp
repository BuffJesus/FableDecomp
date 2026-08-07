#include "rebuild_abi.h"
extern "C" void FABLE_FASTCALL Act_940bd0_A();
extern "C" int FABLE_FASTCALL Act_940bd0_B();
struct C { char p[0x30]; int f30; char p2[0x3c-0x34]; int f3c; void OnActivate(); };
void C::OnActivate(){ this->f3c++; Act_940bd0_A(); this->f30 = Act_940bd0_B(); }