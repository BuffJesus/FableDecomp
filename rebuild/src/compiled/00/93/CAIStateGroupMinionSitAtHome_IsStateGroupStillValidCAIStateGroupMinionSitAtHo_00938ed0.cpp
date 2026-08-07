#include "rebuild_abi.h"
extern "C" void FABLE_FASTCALL Val_938ed0_A(void* self);
extern "C" int FABLE_FASTCALL Val_938ed0_B();
struct C { char p[0x3c]; int f3c; char Valid(); };
char C::Valid(){ Val_938ed0_A(this); int b=Val_938ed0_B(); return (char)(b <= this->f3c); }