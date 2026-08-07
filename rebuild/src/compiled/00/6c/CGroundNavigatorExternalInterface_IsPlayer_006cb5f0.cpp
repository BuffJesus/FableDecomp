#include "rebuild_abi.h"
struct S { char p[0x10]; unsigned char f10; };
struct C { char p[4]; S* f4; bool IsPlayer(); };
bool C::IsPlayer(){ return this->f4->f10==2; }