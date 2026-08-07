#include "rebuild_abi.h"
struct C { char p[0xc]; void* fc; void* f10; bool Free(); };
bool C::Free(){ return this->fc != this->f10; }