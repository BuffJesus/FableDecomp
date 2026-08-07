#include "rebuild_abi.h"
struct S { char p[0x5c]; unsigned char f5c; };
struct C { char p[4]; S* f4; bool Can(int a); };
bool C::Can(int a){ return this->f4->f5c==0; }