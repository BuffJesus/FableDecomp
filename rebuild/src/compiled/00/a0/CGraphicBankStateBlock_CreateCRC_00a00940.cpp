#include "rebuild_abi.h"
struct C { int f0; int f4; int Helper(void* p, int n); int CreateCRC(); };
int C::CreateCRC(){ return this->Helper(&this->f4, 0x60); }