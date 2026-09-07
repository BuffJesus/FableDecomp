#pragma optimize("s",on)
#include <cstdlib>
void** __fastcall ListNode_AllocateCircularNode(void** outNode,void* unusedRegisterArgument,void* unusedStackArgument){(void)unusedRegisterArgument;(void)unusedStackArgument;*(unsigned long*)outNode&=0;*outNode=std::malloc(0x18);return outNode;}