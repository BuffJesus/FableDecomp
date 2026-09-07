#pragma optimize("s",on)
#include <stdlib.h>
struct CircularNode_0042abaf{CircularNode_0042abaf* next;CircularNode_0042abaf* previous;};void** __fastcall list(void** out,void* unusedRegister,void* unusedStack){(void)unusedRegister;(void)unusedStack;*(unsigned long*)out&=0;CircularNode_0042abaf* node=(CircularNode_0042abaf*)malloc(0xc);node->next=node;node->previous=node;*out=node;return out;}