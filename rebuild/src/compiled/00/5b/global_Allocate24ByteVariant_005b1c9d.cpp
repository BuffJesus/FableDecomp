#pragma optimize("s",on)
#include <cstdlib>
void** __fastcall Allocate_24Byte_Variant(void** outNode,void* unusedRegisterArgument,void* unusedStackArgument){(void)unusedRegisterArgument;(void)unusedStackArgument;*(unsigned long*)outNode&=0;*outNode=std::malloc(0x18);return outNode;}