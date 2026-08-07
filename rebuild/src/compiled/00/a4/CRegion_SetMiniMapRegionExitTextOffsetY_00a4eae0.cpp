#include "rebuild_abi.h"
struct R { char p[0x14]; float f14; };
struct Sub { R* Get(void* a); };
struct C { char p[0x20]; Sub s20; float Set(void* a); };
float C::Set(void* a){ return this->s20.Get(a)->f14; }