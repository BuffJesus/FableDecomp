#include "rebuild_abi.h"
struct G { int Helper(); };
extern G* g_908de0;
struct C { char pad[0x28]; int f28; void OnStart(); };
void C::OnStart(){ this->f28 = g_908de0->Helper(); }