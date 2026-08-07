#include "rebuild_abi.h"
struct C { char p[276]; unsigned char f; int R(); };
int C::R(){ return this->f==0; }