#include "rebuild_abi.h"
struct Sub { void Helper(int n); };
struct C { char p24[0x24]; Sub* f24; char p28[0x3c-0x28]; char f3c; void Cleanup(); };
void C::Cleanup(){ this->f24->Helper(0x14); this->f3c=0; }