#pragma optimize("s",on)
#include <cstdlib>
void** __fastcall Allocate_20Bytes(void** outNode,void* unusedRegisterArgument,void* unusedStackArgument){(void)unusedRegisterArgument;(void)unusedStackArgument;*(unsigned long*)outNode&=0;*outNode=std::malloc(0x14);return outNode;}