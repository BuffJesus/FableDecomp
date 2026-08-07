#include "rebuild_abi.h"
struct S { char p[0x5c]; unsigned char f5c; };
struct C { char p[0x40]; S* f40; bool Can(int a); };
bool C::Can(int a){ return this->f40->f5c==0; }