#include "rebuild_abi.h"
struct M { bool Check(); };
struct A { char p[0x64]; M* f64; };
struct B { char p[0x18]; A* f18; };
struct C { char p[4]; B* f4; char Is(); };
char C::Is(){ bool r=this->f4->f18->f64->Check(); return (char)(r!=false); }