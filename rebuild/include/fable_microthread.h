#pragma once
#include "rebuild_abi.h"

extern fable_u8* g_FableMicroThreadStackPointer_013CA8A0;
extern fable_u8* g_FableMicroThreadStackBase_013CA8A4;
extern unsigned long g_FableMicroThreadStackSize_013CA89C;

namespace MicroThread
{
    void FABLE_FASTCALL SetStack(fable_u8* stackTop, unsigned long stackSize);
}
