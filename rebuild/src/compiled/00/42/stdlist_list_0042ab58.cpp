#pragma optimize("s",on)
#include <stdlib.h>
struct CircularNode_0042ab58{CircularNode_0042ab58* next;CircularNode_0042ab58* previous;};void** __fastcall list(void** out,void* unusedRegister,void* unusedStack){(void)unusedRegister;(void)unusedStack;*(unsigned long*)out&=0;CircularNode_0042ab58* node=(CircularNode_0042ab58*)malloc(0xc);node->next=node;node->previous=node;*out=node;return out;}