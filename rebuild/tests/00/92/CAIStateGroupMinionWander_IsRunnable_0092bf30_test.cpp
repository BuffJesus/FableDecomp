#include "rebuild_abi.h"
#include <cstdio>
struct M { bool Check(); };
struct A { char p[0x64]; M* f64; };
struct B { char p[0x18]; A* f18; };
struct C { char p[4]; B* f4; char Is(); };
static bool cv=false; bool M::Check(){ return cv; }
char C::Is(){ bool r=this->f4->f18->f64->Check(); return (char)(r!=false); }
int main(){ M m; A a; a.f64=&m; B b; b.f18=&a; C o; o.f4=&b; cv=true; if(o.Is()!=1){std::printf("B1");return 1;} cv=false; if(o.Is()!=0){std::printf("B2");return 1;} std::printf("RB_92bf30_OK\n"); return 0; }