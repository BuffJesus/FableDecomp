#include "rebuild_abi.h"
struct C { char p[0x28]; void* f28; void Add(void* a); void Helper(void* a); };
void C::Add(void* a){ this->f28=a; this->Helper(0); }