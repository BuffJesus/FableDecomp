#include "rebuild_abi.h"
struct C { char p10[0x10]; int f10; char p14[0x1c-0x14]; int f1c; };
extern "C" char FABLE_FASTCALL FableRet_85b8a0(C *self){ return (char)((self->f1c + 1) < self->f10); }