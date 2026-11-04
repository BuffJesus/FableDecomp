// Retail 009D86B0; six instructions, also matched to Ego_r SetStack.
#include "fable_microthread.h"

void FABLE_FASTCALL MicroThread::SetStack(fable_u8* stackTop, unsigned long stackSize)
{
    fable_u8* stackPointer = stackTop - sizeof(unsigned long);
    *reinterpret_cast<unsigned long*>(stackPointer) = 0xCDCDCDCDUL;
    g_FableMicroThreadStackPointer_013CA8A0 = stackPointer;
    g_FableMicroThreadStackBase_013CA8A4 = stackTop;
    g_FableMicroThreadStackSize_013CA89C = stackSize;
}