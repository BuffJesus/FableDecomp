#include "rebuild_abi.h"
#include <cstdio>
static int bv=0;
extern "C" void FABLE_FASTCALL Val_938ed0_A(void* self){}
extern "C" int FABLE_FASTCALL Val_938ed0_B(){ return bv; }
struct C { char p[0x3c]; int f3c; char Valid(); };
char C::Valid(){ Val_938ed0_A(this); int b=Val_938ed0_B(); return (char)(b <= this->f3c); }
int main(){ C o; o.f3c=5; bv=3; if(!o.Valid()){std::printf("B1");return 1;} bv=9; if(o.Valid()){std::printf("B2");return 1;}
 std::printf("RB_938ed0_OK\n"); return 0; }