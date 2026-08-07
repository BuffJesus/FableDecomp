#include "rebuild_abi.h"
#include <cstdio>
struct M { bool Check(); };
static bool cv=false; bool M::Check(){ return cv; }
static M gm;
extern "C" M* FABLE_FASTCALL Enemy_954130_A(void* self){ return &gm; }
struct C { char p[0x14]; char s14; char Has(); };
char C::Has(){ M* m=Enemy_954130_A((char*)this+0x14); bool r=m->Check(); return (char)(r!=false); }
int main(){ C o; cv=true; if(!o.Has()){std::printf("B1");return 1;} cv=false; if(o.Has()){std::printf("B2");return 1;}
 std::printf("RB_954130_OK\n"); return 0; }