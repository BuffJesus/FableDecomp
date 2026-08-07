#include "rebuild_abi.h"
struct G { int Helper(); };
extern G* g_657280;
struct C { char pad[0x2c]; int f2c; void Bought(); };
void C::Bought(){ this->f2c = g_657280->Helper(); }