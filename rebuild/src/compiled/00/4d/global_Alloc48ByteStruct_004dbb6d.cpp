#pragma optimize("s",on)
#include <cstdlib>
void** __fastcall Alloc_48ByteStruct(void** outNode,void* unusedRegisterArgument,void* unusedStackArgument){(void)unusedRegisterArgument;(void)unusedStackArgument;*(unsigned long*)outNode&=0;*outNode=std::malloc(0x30);return outNode;}