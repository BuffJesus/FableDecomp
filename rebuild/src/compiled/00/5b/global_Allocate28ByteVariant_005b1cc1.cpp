#pragma optimize("s",on)
#include <cstdlib>
void** __fastcall Allocate_28Byte_Variant(void** outNode,void* unusedRegisterArgument,void* unusedStackArgument){(void)unusedRegisterArgument;(void)unusedStackArgument;*(unsigned long*)outNode&=0;*outNode=std::malloc(0x1c);return outNode;}