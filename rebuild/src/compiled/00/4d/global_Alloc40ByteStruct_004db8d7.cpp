#pragma optimize("s",on)
#include <cstdlib>
void** __fastcall Alloc_40ByteStruct(void** outNode,void* unusedRegisterArgument,void* unusedStackArgument){(void)unusedRegisterArgument;(void)unusedStackArgument;*(unsigned long*)outNode&=0;*outNode=std::malloc(0x28);return outNode;}