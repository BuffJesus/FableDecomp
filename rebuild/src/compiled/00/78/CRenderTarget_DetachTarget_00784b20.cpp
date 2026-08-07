#include "rebuild_abi.h"
struct C { char p[0x2c]; void* f2c; void Detach(); void Helper(); };
void C::Detach(){ if(this->f2c) this->Helper(); }