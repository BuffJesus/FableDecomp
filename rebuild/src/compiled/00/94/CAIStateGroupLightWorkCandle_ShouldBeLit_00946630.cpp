#include "rebuild_abi.h"
struct M { bool Check(); };
struct S { char p[0x1c]; void* f1c; };
extern "C" S* FABLE_FASTCALL GetS_946630();
extern "C" bool FABLE_FASTCALL Should_946630(){ return !((M*)(GetS_946630()->f1c))->Check(); }